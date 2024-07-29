#if defined(WASAPI)

#include "AudioOutWASAPI.h"
#include "stdio.h" // for printf
#include "math.h"
#include "Mmdeviceapi.h"
#include <iostream>
#include <Functiondiscoverykeys_devpkey.h>

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);

//for freeing memory
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

//time units used by WASAPI
#define REFTIMES_PER_SEC 10000000

AudioOutWASAPI::AudioOutWASAPI()
{
	m_outputClient = NULL;
	m_inputClient = NULL;
	m_WASRender = NULL;
	m_WASCapture = NULL;
	outputBufferSize = 0;
	frameLen = 0;
	m_running = false;

	m_hWnd = NULL;
	m_hNotifyEvent = NULL;
	memset(fillProxy, 0, sizeof(fillProxy));

	memset(proxy_buf.bufferA, 0, sizeof(proxy_buf.bufferA));
	memset(proxy_buf.bufferB, 0, sizeof(proxy_buf.bufferB));

	proxy_buf.writePtr = &(proxy_buf.bufferA[0]);
	proxy_buf.readPtr = &(proxy_buf.bufferA[0]);
	proxy_buf.indW = 0;
	proxy_buf.indR = 0;
	proxy_buf.buffW = 0;
	proxy_buf.buffR = 0;

	m_hThread = NULL;
	m_iThread = NULL;
	priorityTask = NULL;
}

//Converts HRESULT errors into string format
void ErrorDescription(HRESULT hr)
{
	if (FACILITY_WINDOWS == HRESULT_FACILITY(hr))
		hr = HRESULT_CODE(hr);
	TCHAR* szErrMsg;

	if (FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&szErrMsg, 0, NULL) != 0)
	{
		printf("%s", (char *)szErrMsg);
		LocalFree(szErrMsg);
	}
	else
		printf("[Could not find a description for error # %#x.]\n", hr);
}

void AudioOutWASAPI::Initialize(void(*aStepfunction)(), float* inputBuff, float *outputBuff, DWORD numChannels,
	DWORD deviceFrameSamples, DWORD sampleRate,
	DWORD modelFrameSamples)
{
	stepfcn = aStepfunction;
	SysInPtr = inputBuff;
	SysOutPtr = outputBuff;

	DWORD bitsPerSample = 32;
	DWORD blockAlignment = numChannels * bitsPerSample / 8;

	m_samplesPerBlock = deviceFrameSamples;
	m_bytesPerBlock = m_samplesPerBlock * blockAlignment;
	m_waveFormat.wFormatTag = WAVE_FORMAT_IEEE_FLOAT; // WAVE_FORMAT_PCM; 	//C:\Program Files (x86)\Windows Kits\8.1\Include\shared\mmreg.h
	m_waveFormat.nChannels = (WORD)numChannels;
	m_waveFormat.nSamplesPerSec = sampleRate;
	m_waveFormat.nAvgBytesPerSec = sampleRate * blockAlignment;
	m_waveFormat.wBitsPerSample = (WORD)bitsPerSample;
	m_waveFormat.nBlockAlign = (WORD)blockAlignment;
	m_waveFormat.cbSize = 0;

	inputFrameSize = modelFrameSamples;

	//length of each frame with respect to reference time
	REFERENCE_TIME refTimesPerSample = REFTIMES_PER_SEC / m_waveFormat.nSamplesPerSec + 1;
	frameLen = refTimesPerSample * m_samplesPerBlock * m_waveFormat.nChannels;

	//Initialize COM port for enumerating audio devices
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	HWND hWnd = GetConsoleWindow();
	HRESULT hres;

	//Get a list of available audio input and output devices
	hres = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&m_pEnumerator);
	if (FAILED(hres)) {
		printf("CoCreateInstance failed. Unable to get list of audio input and output devices\n");
	}

    ////////////////////////////////////////////////////
    //Get output device from command line input
    m_outputDevice = this->enumerateAndChooseDevice(eRender);
    //Get input device from command line input
    m_inputDevice = this->enumerateAndChooseDevice(eCapture);
    ////////////////////////////////////////////////////

	//attach output endpoint device to render client
	hres = m_outputDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&m_outputClient);
	if (FAILED(hres)) {
		printf("Activate failed for render client. Unable to attach audio output device to WASAPI client\n");
	}

	//attach input endpoint device to capture client
	hres = m_inputDevice->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&m_inputClient);
	if (FAILED(hres)) {
		printf("Activate failed for capture client. Unable to attach audio input device to WASAPI client\n");
	}

	//Compare requested format to audio input/output device formats (useful for debugging)
	//sample rates, sample size and num channels must match
	WAVEFORMATEX *outFormat = NULL;
	WAVEFORMATEX *inFormat = NULL;
	hres = m_outputClient->GetMixFormat(&outFormat);
	hres = m_inputClient->GetMixFormat(&inFormat);

    printf("--------------- Device summary ----------------\n");
	printf("FormatTag- model: %d, out: %d, in: %d\n", m_waveFormat.wFormatTag, outFormat->wFormatTag, inFormat->wFormatTag);
	printf("Channels- model: %d, out: %d, in: %d\n", m_waveFormat.nChannels, outFormat->nChannels, inFormat->nChannels);
	printf("Sample Rate(Hertz)- model: %d, out: %d, in: %d\n", m_waveFormat.nSamplesPerSec, outFormat->nSamplesPerSec, inFormat->nSamplesPerSec);
	printf("Sample Rate(bytes)- model: %d, out: %d, in: %d\n", m_waveFormat.nAvgBytesPerSec, outFormat->nAvgBytesPerSec, inFormat->nAvgBytesPerSec);
	printf("Sample Size(bits)- model: %d, out: %d, in: %d\n", m_waveFormat.wBitsPerSample, outFormat->wBitsPerSample, inFormat->wBitsPerSample);
	printf("Block Align- model: %d, out: %d, in: %d\n", m_waveFormat.nBlockAlign, outFormat->nBlockAlign, inFormat->nBlockAlign);
	printf("CB Size- model: %d, out: %d, in: %d\n\n", m_waveFormat.cbSize, outFormat->cbSize, inFormat->cbSize);

	//Initialize audio render client in callback mode
	hres = m_outputClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_EVENTCALLBACK, frameLen,
		frameLen, &m_waveFormat, NULL);
	if (FAILED(hres)) {
		printf("Initialize failed for audio render client. Check to make sure sample rates of devices match\n");
		ErrorDescription(hres);
	}

	//Initalize audio capture client
	REFERENCE_TIME hnsDefaultDevicePeriod;
	hres = m_inputClient->GetDevicePeriod(&hnsDefaultDevicePeriod, NULL);
	hres = m_inputClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, frameLen * 4,
		0, &m_waveFormat, NULL);
	if (FAILED(hres)) {
		printf("Initialize failed for audio capture client.  Check to make sure sample rates of devices match\n");
		ErrorDescription(hres);
	}

	//Get actual size of output buffer
	//Actual buffer size gauranteed >= requested size
	hres = m_outputClient->GetBufferSize(&outputBufferSize);
	if (FAILED(hres)) {
		printf("GetBuffer failed for audio render client\n");
		ErrorDescription(hres);
	}

	//signaled whenever the output device requests a block of audio
	m_hNotifyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (m_hNotifyEvent == NULL)
	{
		printf("error creating event for m_hnotify\n");
	}

	//fillProxy[0] signaled when bufferA has been read
	//fillProxy[1] signaled when bufferB has been read
	//fillProxy[0] signaled when bufferA has been fully written
	//fillProxy[1] signaled when bufferB has been fully written
	fillProxy[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
	fillProxy[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
	fillProxy[2] = CreateEvent(NULL, FALSE, FALSE, NULL);
	fillProxy[3] = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (fillProxy[0] == NULL || fillProxy[1] == NULL || 
		fillProxy[2] == NULL || fillProxy[3] == NULL)
	{
		printf("error creating events for fillProxy\n");
	}

	//set event callback for output buffer
	hres = m_outputClient->SetEventHandle(m_hNotifyEvent);
	if (FAILED(hres)) {
		printf("SetEventHandle failed. Unable to set event callback handle for audio render client\n");
		ErrorDescription(hres);
	}

	//Attach renderer object to audio client
	hres = m_outputClient->GetService(IID_IAudioRenderClient, (void**)&m_WASRender);
	if (FAILED(hres)) {
		printf("GetService for audio render failed. Unable to attach buffer to render client\n");
		ErrorDescription(hres);
	}

	//Attach capturer object to audio client
	hres = m_inputClient->GetService(IID_IAudioCaptureClient, (void **)&m_WASCapture);
	if (FAILED(hres)) {
		printf("GetService for audio capture failed. Unable to attach buffer to capture client\n");
		ErrorDescription(hres);
	}

}


//destructor
AudioOutWASAPI::~AudioOutWASAPI()
{
	SAFE_RELEASE(m_WASRender);
    SAFE_RELEASE(m_WASCapture);
    SAFE_RELEASE(m_outputDevice);
    SAFE_RELEASE(m_inputDevice);
    SAFE_RELEASE(m_outputClient);
    SAFE_RELEASE(m_inputClient);
    SAFE_RELEASE(m_pEnumerator);

	CloseHandle(m_hWnd);
	CloseHandle(m_hNotifyEvent);
	CloseHandle(fillProxy[0]);
	CloseHandle(fillProxy[1]);
	CloseHandle(fillProxy[2]);
	CloseHandle(fillProxy[3]);
	//CloseHandle(priorityTask); // TODO: Causes crash on some systems
}

IMMDevice* AudioOutWASAPI::enumerateAndChooseDevice(EDataFlow captureOrRender)
{
    IMMDevice* pDevice;

    // Variables for performing device enumeration
    IMMDeviceCollection *ppDevices;
    UINT numDevices;
    UINT32 deviceIndex = -1;
    LPWSTR name;
    PROPVARIANT varName;
    IPropertyStore *pProps = NULL;
    HRESULT hres;
    const char* devType = (captureOrRender == eRender) ? "output" : "input";

    //Get device from command line input
    hres = m_pEnumerator->EnumAudioEndpoints(captureOrRender, DEVICE_STATE_ACTIVE, &ppDevices);
    if (FAILED(hres)) {
        printf("EnumAudioEndpoints failed, unable to enumerate %s devices\n", devType);
    }
    ppDevices->GetCount(&numDevices);
    printf("-------------------------- %s ----------------------\n",devType);
    printf("Found %d audio %s devices\n", numDevices, devType);
    for (UINT32 i = 0; i < numDevices; i++) //print available devices
    {
        ppDevices->Item(i, &pDevice);
        pDevice->GetId(&name);
        pDevice->OpenPropertyStore(STGM_READ, &pProps);
        PropVariantInit(&varName);
        pProps->GetValue(PKEY_Device_FriendlyName, &varName);
        std::wcout << "Device " << i << ": " << varName.pwszVal << "\n";
    }
    while (!(deviceIndex >= 0 && deviceIndex  < numDevices))
    {
        printf("Enter a device index to use for %s (default device is 0): ", devType);
        std::cin >> deviceIndex;

        if (!(deviceIndex >= 0 && deviceIndex  < numDevices))
        {
            std::cout << "Invalid device index entered\n";
        }
    }
    ppDevices->Item(deviceIndex, &pDevice);
    return pDevice;
}



bool AudioOutWASAPI::Start()
{
	HRESULT hres;

	if (!stepfcn)
	{
		printf("\n** AudioOut Start failed - not Initialized yet\n");
		return false;
	}

	bool success = true;
	m_running = true;

	UINT32 numFramesAvailable = 0;
	UINT32 numFramesPadding = 0;
	UINT32 capturePacketSize = 0;
	UINT32 numSent = 0;
	DWORD flags;
	BYTE *capturePtr;


	//start recording
	hres = m_inputClient->Start();
	if (FAILED(hres))
	{
		printf("Starting capture failed. Unable to fill input buffer.\n");
		ErrorDescription(hres);
	}

	//fill the first proxy buffer before starting audio output
	m_WASCapture->GetNextPacketSize(&capturePacketSize);

	while (proxy_buf.indW + (capturePacketSize * m_waveFormat.nChannels) <= WASAPI_BUFFER_LENGTH)
	{
		if(capturePacketSize > 0){
			m_WASCapture->GetBuffer(&capturePtr, &numSent, &flags, NULL, NULL);
			memcpy(proxy_buf.writePtr, (float *)capturePtr, numSent * sizeof(float) * m_waveFormat.nChannels);

			proxy_buf.indW += numSent * m_waveFormat.nChannels;
			proxy_buf.writePtr += numSent * m_waveFormat.nChannels;
			hres = m_WASCapture->ReleaseBuffer(numSent);
		}

		m_WASCapture->GetNextPacketSize(&capturePacketSize);
	}
		
	processWriteOverflow(1, proxy_buf.bufferB, numSent);

		//launch thread to fill input buffer
	if (success == true)
	{
		DWORD ThreadID;
		m_iThread = CreateThread(NULL, 0,
			(LPTHREAD_START_ROUTINE)inputThreadFunc,
			this,
			0, &ThreadID);

		if (!m_iThread)
		{
			printf("inputThreadFunc CreateThread failed: error code %d\n", GetLastError());
			success = false;
		}
	}

	//launch thread to process callbacks and fill output buffer
	if (success == true)
	{
		DWORD ThreadID2;
		m_hThread = CreateThread(NULL, 0,
			(LPTHREAD_START_ROUTINE)outputThreadFunc,
			this,
			0, &ThreadID2);

		if (!m_hThread)
		{
			printf("CreateWriteNotification CreateThread failed: error code %d\n", GetLastError());
			success = false;
		}
	}

	success &= BeginPlayback();

	return success;
}

void AudioOutWASAPI::Stop()
{
	StopPlayback();
}


bool AudioOutWASAPI::BeginPlayback()
{
	bool success = true;

	HRESULT hres = m_outputClient->Start();
	if (FAILED(hres)) {
		printf("Start failed. Unable to begin playback.\n");
		ErrorDescription(hres);
		success = false;
	}

	return success;
}

bool AudioOutWASAPI::StopPlayback()
{
	bool success = true;
	m_running = false;

	if (m_hThread)
	{

		// Wait for the output loop thread to finish
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}

	if (m_iThread)
	{
		//Wait for input loop thread to finish
		WaitForSingleObject(m_iThread, INFINITE);
		CloseHandle(m_iThread);
	}

	m_outputClient->Stop();
	return success;
}

//Switches write pointer when next packet will overflow or fill current buffer half
//signals read pointer when it is safe to read the written half
void AudioOutWASAPI::processWriteOverflow(int nextBuffWrite, float *nextWritePtr, UINT32 numSent)
{
	DWORD flags;
	BYTE* capturePtr;

	if (proxy_buf.indW < WASAPI_BUFFER_LENGTH) { //packet causes overflow
		m_WASCapture->GetBuffer(&capturePtr, &numSent, &flags, NULL, NULL);
		memcpy(proxy_buf.writePtr, (float *)capturePtr, sizeof(float) * (WASAPI_BUFFER_LENGTH - proxy_buf.indW));
		proxy_buf.writePtr = nextWritePtr;
		proxy_buf.buffW = nextBuffWrite;
        UINT32 floatsToCopy = min(WASAPI_BUFFER_LENGTH, (proxy_buf.indW + (numSent * m_waveFormat.nChannels) - WASAPI_BUFFER_LENGTH));
		memcpy(proxy_buf.writePtr, ((float *)capturePtr) + (WASAPI_BUFFER_LENGTH - proxy_buf.indW),
			sizeof(float) * floatsToCopy);
		proxy_buf.indW = floatsToCopy;
		proxy_buf.writePtr += proxy_buf.indW;
		m_WASCapture->ReleaseBuffer(numSent);
	}

	else { //packet perfectly fills buffer
		proxy_buf.writePtr = nextWritePtr;
		proxy_buf.buffW = nextBuffWrite;
		proxy_buf.indW = 0;
	}
}

//switches read pointer when current buffer half is done being read
//signals to write pointer when it is safe to write the read half
void AudioOutWASAPI::processReadSwitch(int nextBuffRead, float *nextReadPtr, float *tempPtr)
{
	proxy_buf.indR = 0;
	proxy_buf.readPtr = nextReadPtr;
	proxy_buf.buffR = nextBuffRead;

	//copy the entire buffer half before processing so write pointer can overwrite
	memcpy(tempPtr, proxy_buf.readPtr, sizeof(float) * WASAPI_BUFFER_LENGTH);
	proxy_buf.readPtr = tempPtr;
}

//processes audio input data and sends it to proxy buffer
void AudioOutWASAPI::inputBufferLoop()
{
	HRESULT hres;
	BYTE* capturePtr;
	UINT32 packetLength = 0;
	UINT32 numSent = 0;
	DWORD flags;
	size_t bytesToProcess = 0;
	
	while (m_running)
	{
		//packetLength dictated by WASAPI, not alterable
		m_WASCapture->GetNextPacketSize(&packetLength);

		//If next packet will overflow current buffer, switch to next buffer when it is available
		if (proxy_buf.indW + (packetLength * m_waveFormat.nChannels) > WASAPI_BUFFER_LENGTH) {

			if (proxy_buf.buffW == 0) {

				WaitForSingleObject(fillProxy[1], 1000); //Wait for bufferB to be read

				processWriteOverflow(1, proxy_buf.bufferB, numSent);

				SetEvent(fillProxy[2]); //Signal that bufferA has been written

			}

			else{
				WaitForSingleObject(fillProxy[0], 1000); //Wait for bufferA to be read

				processWriteOverflow(0, proxy_buf.bufferA, numSent);

				SetEvent(fillProxy[3]); //Signal that bufferB has been written
			}
		}

		//copy audio input data to proxy buffer
		else if(packetLength > 0){
			hres = m_WASCapture->GetBuffer(&capturePtr, &numSent, &flags, NULL, NULL);
			memcpy(proxy_buf.writePtr, (float *)capturePtr, numSent * sizeof(float) * m_waveFormat.nChannels);

			proxy_buf.indW += numSent * m_waveFormat.nChannels;
			proxy_buf.writePtr += numSent * m_waveFormat.nChannels;

			hres = m_WASCapture->ReleaseBuffer(numSent);
		}

	}
}

//sends data from proxy buffer to simulink step function
//sends result to output buffer
void AudioOutWASAPI::outputBufferLoop()
{
    UINT32 stepCount = 0;
	UINT32 flags = 0;
	UINT32 numFramesAvailable;
	UINT32 numFramesPadding;
	UINT32 packetLength = 0;
	UINT32 numSent = 0;
	HRESULT hres;
	BYTE *buffPtr;
	bool play = true;
	int outInd = 0;

	float temp[WASAPI_BUFFER_LENGTH];
	memcpy(temp, proxy_buf.readPtr, sizeof(float) * WASAPI_BUFFER_LENGTH);
	SetEvent(fillProxy[0]);
	proxy_buf.readPtr = temp;

	while (m_running)
	{
		//Wait for buffer signal
		WaitForSingleObject(m_hNotifyEvent, 2000);

		m_outputClient->GetCurrentPadding(&numFramesPadding);
		numFramesAvailable = outputBufferSize - numFramesPadding;

		if (numFramesAvailable >= m_samplesPerBlock) {
			hres = m_WASRender->GetBuffer(m_samplesPerBlock, &buffPtr);
			if (FAILED(hres)) {
				printf("GetBuffer failed. Unable to grab next data packet from render buffer\n");
				ErrorDescription(hres);
			}

			//process several frames of simulink model audio to fill WASAPI's device frame size
			for (BYTE* b = buffPtr; b < buffPtr + (m_samplesPerBlock * m_waveFormat.nBlockAlign); b += (inputFrameSize * m_waveFormat.nBlockAlign))
			{
				//Uninterleave buffer data for simulink processing
				for (UINT32 f = 0; f < inputFrameSize; f++)
				{
					for (int c = 0; c < m_waveFormat.nChannels; c++)
					{
						SysInPtr[c * inputFrameSize + f] = proxy_buf.readPtr[0];
						proxy_buf.indR++;
						proxy_buf.readPtr = proxy_buf.readPtr + 1;

					}
				}

				//process a simulink frame using the simulink step function
				this->stepfcn();
                stepCount++;

				//Interleave output data for Windows audio output
				outInd = 0;
				for (UINT32 f = 0; f < inputFrameSize; f++)
				{
					for (UINT32 c = 0; c < m_waveFormat.nChannels; c++)
					{
						((float *)b)[outInd] = SysOutPtr[c * inputFrameSize + f];
						outInd++;
					}
				}
			}

			if (proxy_buf.indR >= WASAPI_BUFFER_LENGTH && proxy_buf.buffR == 0) {

				WaitForSingleObject(fillProxy[3], 1000); //wait for bufferB to be written

				processReadSwitch(1, proxy_buf.bufferB, temp);

				SetEvent(fillProxy[1]); //signal that bufferB has been read
			}

			else if (proxy_buf.indR >= WASAPI_BUFFER_LENGTH && proxy_buf.buffR == 1) {

				WaitForSingleObject(fillProxy[2], 1000); //wait for bufferA to be written

				processReadSwitch(0, proxy_buf.bufferA, temp);

				SetEvent(fillProxy[0]); //signal that bufferA has been read
			}

			hres = m_WASRender->ReleaseBuffer(m_samplesPerBlock, 0);
			if (FAILED(hres)) {
				printf("ReleaseBuffer failed. Unable to release lock on buffer.\n");
				ErrorDescription(hres);
			}
		}
	}
}

//launch output buffer loop
void AudioOutWASAPI::outputThreadFunc(AudioOutWASAPI* theGeneratorIn)
{

	//Boost priority for buffer filling thread for duration of the stream
	DWORD taskIndex = 0;
	theGeneratorIn->priorityTask = AvSetMmThreadCharacteristics(TEXT("Pro Audio"), &taskIndex);
	if (theGeneratorIn->priorityTask == NULL)
	{
		printf("failed to raise thread priority for output buffer loop");
	}

	theGeneratorIn->outputBufferLoop();

	if (theGeneratorIn->priorityTask != NULL)
	{
		AvRevertMmThreadCharacteristics(theGeneratorIn->priorityTask);
	}

}

//launch input buffer loop
void AudioOutWASAPI::inputThreadFunc(AudioOutWASAPI* theGeneratorIn)
{
	theGeneratorIn->inputBufferLoop();
}

#endif