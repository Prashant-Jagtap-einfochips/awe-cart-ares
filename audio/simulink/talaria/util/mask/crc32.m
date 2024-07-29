function [crc32] = crc32(input_string)
% crc32: Compute 32-bit crc of input string
%
% CRC = crc32(INPUT)
%   INPUT: ascii string to use for CRC32 computation
%     CRC: 32-bit CRC represented as hex string
%
% NOTE
%   This was verified against python's binascii.crc32() implementation
%

%   Copyright 2018-2019 Bose Corporation
% General variables
debug = 0;
crc_length = 32;
raw_polynomial = '104C11DB7';
FFs = 'ffffffff';
FFs_binary = dec2bin(hex2dec(FFs)) - '0';
input_reversed = [];

% Compute 'polynomial', which is a massaged version of the raw polynomial
% to be used in the computation
% @reference: http://sourceforge.net/p/parchive/mailman/message/1746090/
polynomial_length = crc_length + 1;
raw_bin_polynomial = dec2bin(hex2dec(raw_polynomial)) - '0';
zero_pad = zeros(1,polynomial_length - length(raw_bin_polynomial));
polynomial = [raw_bin_polynomial zero_pad];

% Compute 'input', which is the input string converted to binary, flipped  
% byte-by-byte and zero padded to be used for computation
% @reference: http://stackoverflow.com/questions/5047494/python-crc-32-woes
input_binary = char2bin(input_string);
index = 1;
while (index < length(input_binary))
    next_byte = input_binary(index:index+7);
    next_byte_flipped = fliplr(next_byte);
    input_reversed(index:index+7) = next_byte_flipped;
    index = index + 8;
end
input_padded = [input_reversed zeros(1,crc_length)];
input = xor(input_padded(1:crc_length), FFs_binary); 
input = [input input_padded(crc_length + 1:end)];
if debug
    disp(['Input:      ' bin2char(input)]);
end

% Perform modulo-2 binary division
% @reference: http://en.wikipedia.org/wiki/Cyclic_redundancy_check
% @reference: http://en.wikipedia.org/wiki/Computation_of_cyclic_redundancy_checks
dividend = input;
index = find(dividend, 1);
final_index = length(input) - polynomial_length + 2;
while (index < final_index)
    % xor the 32 bits starting at index
    msb_to_copy = dividend(1:index - 1);
    lsb_to_copy = dividend(index + polynomial_length:end);
    bits_to_xor = dividend(index:index + polynomial_length - 1);

    xor_result = xor(bits_to_xor, polynomial);
    dividend = [msb_to_copy xor_result lsb_to_copy];
    
    if debug
        spaces = repmat(' ', [1 index-1]);
        disp(['Polynomial: ' spaces bin2char(polynomial)]);
        disp(['Dividend:   ' bin2char(dividend)]);
    end
    
    % Increment the index to the next non-zero bit
    index = index + 1;
    index = index + find(dividend(index:end), 1) - 1;
end

remainder = dividend(end - crc_length + 1:end);
if debug
    disp(['Remainder:  ' bin2hex(remainder)]);
end

remainder_xored = xor(remainder, FFs_binary);
crc32 = bin2hex(fliplr(remainder_xored));


function bin = char2bin(input_in_char)
% Converts character string to binary array
bits_per_char = 8;
num_chars = length(input_in_char);
input_in_ascii = double(input_in_char);
input_in_binary = dec2bin(input_in_ascii, bits_per_char);
input_in_binary_1d = reshape(input_in_binary', [1 num_chars * bits_per_char]);
input_in_binary_1d_array = input_in_binary_1d - '0';
bin = input_in_binary_1d_array;

function hex = bin2hex(bin)
% Converts binary array to hex string
str = bin2char(bin);
dec = bin2dec(str);
hex = dec2hex(dec);

function str = bin2char(bin)
% converts binary array to binary character array ('1' or '0') 
str = num2str(bin);
str(isspace(str)) = '';
