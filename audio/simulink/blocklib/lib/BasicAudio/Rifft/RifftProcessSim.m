function [y, lastFrame] = RifftProcessSim(u, win, num_overlap, lastFrame)

% Restore conjugate symmetry
newPart = u(2:end-1, :);         % Get N-2 middle points 
newPart = flipud(conj(newPart)); % Conjugate and flip
y = [u; newPart];                % Concat to get full spectrum

% Perform ifft
y = ifft(y); 

% Window and overlap-add
% Note: This function creates another outputOverlap buffer since Matlab
% variables can only be passed by value
[y, lastFrame] = winOverlapAdd(y, win, num_overlap, lastFrame);

end

function [y, lastFrame] = winOverlapAdd(u, win, numOverlap, lastFrame)

% Get sizes
[N, numChannels] = size(u);
L = N - numOverlap;

% Cast to real and apply window
% Loop since array-vector multiply operation doesn't work here
u = real(u);
for i = 1:numChannels
    u(:, i) = u(:,i) .* win;
end

% Overlap and add
u(1:numOverlap,:) = u(1:numOverlap,:) + lastFrame;

% Output first L samples
y = u(1:L, :); 

% Update buffer with the other N-L samples
lastFrame = u(L+1: end, :);

end