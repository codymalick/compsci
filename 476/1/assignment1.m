lambda = 2
mu = 1

arriv_mean = 1/lambda
process_mean = 1/mu

% number of frames
frames = 10

% number of simulations:
simulations = 100

results = zeros(simulations,frames)


for i = 1:simulations
    for j = 1:frames
        % Generate queuing delay + transmission delay
        results(i,j) = exprnd(lambda) + exprnd(mu)
    end
end

%plot(1:simulations,results,'+')

% Problem 1
w = sum(results)
wbar = sum(w)/simulations

% Problem 2
