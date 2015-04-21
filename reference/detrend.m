function y = detrend(x,o,bp)
%DETREND Remove a linear trend from a vector, usually for FFT processing.
%   Y = DETREND(X) removes the best straight-line fit linear trend from the
%   data in vector X and returns the residual in vector Y.  If X is a
%   matrix, DETREND removes the trend from each column of the matrix.
%
%   Y = DETREND(X,'constant') removes just the mean value from the vector X,
%   or the mean value from each column, if X is a matrix.
%
%   Y = DETREND(X,'linear',BP) removes a continuous, piecewise linear trend.
%   Breakpoint indices for the linear trend are contained in the vector BP.
%   The default is no breakpoints, such that one single straight line is
%   removed from each column of X.
%
%   Class support for inputs X,BP:
%      float: double, single
%
%   See also MEAN

%   Copyright 1984-2012 The MathWorks, Inc.

if nargin < 2
    o  = 'linear';
end
if nargin < 3
    bp = 1;
end
isrowx = isrow(x);
if isrowx
    x = x(:);   % If a row, turn into column vector
end


switch o
    case {0,'c','constant'}
        y = bsxfun(@minus,x,mean(x,1));   % Remove just mean from each column

    case {1,'l','linear'}
        N = size(x,1);
        bp = unique([1; double(bp(:)); N]);   % Include both endpoints
        bp = bp(bp >= 1 & bp <=N);   % Should error in the future
        lbp = length(bp);
        % Build regressor with linear pieces + DC
        a = zeros(N,lbp,class(x));
        a(1:N,1) = (1:N)./N;
        for k = 2:(lbp-1)
            M = N - bp(k);
            a((bp(k)+1):end,k) = (1:M)./M;
        end
        a(1:N,end) = 1;
        y = x - a*(a\x);   % Remove best fit

    otherwise
        % This should eventually become an error.
        warning(message('MATLAB:detrend:InvalidTrendType', num2str( o )));
        y = detrend(x,'linear',bp);
end

if isrowx
    y = y.';
end
