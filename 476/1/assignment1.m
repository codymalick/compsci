lambda = 2
mu = 1

arriv_mean = 1/lambda
process_mean = 1/mu

% number of frames
frames = 10

% number of simulations:
simulations = 100

results = zeros(simulations,frames)
% delaylist = zeros(simulations,frames)

for i = 1:simulations
    queue = 0
    interarrival = 0
    prev_queue = 0
    prev_trans = 0
    for j = 1:frames
        % Get a transmission time. We need this every round
        transmission = exprnd(mu)
       
        % For the first packet, delay/interarrival are zero, else, get 
        % interarrival and queuing delay
        if j == 1
            queue = 0
            interarrival = 0
        else
            interarrival = exprnd(lambda)
            % queuing delay = previous transmission + previous queuing
            % delay - new interarrival time
            queue = prev_trans + (prev_queue-interarrival)
            
            %don't want negative delay
            if queue < 0
                queue = 0
            end
        end
                    
        % sum queuing delay + transmission delay
        results(i,j) = queue + transmission
        
        % Save results for next run
        prev_queue = queue
        prev_trans = transmission
    end
end

% results of x runs
% Problem 1.a

% average of each simluation
w = sum(results,2)

running_average = zeros(1,simulations)
upper_running_CI = zeros(1,simulations)
lower_running_CI = zeros(1,simulations)
lower_error = zeros(1,simulations)

rowwise_SD = std(results,0,2)


for i = 1:simulations
    running_average(i) = sum(w(1:i))/i 
end

% 90 percent confidence interval
z_crit = 1.645

% Calculate the confidence interval at n simulations
for i = 1:simulations
    running_error(i) = rowwise_SD(i)/(sqrt(10))
    upper_running_CI = running_average(i)+(z_crit*running_error(i))
    lower_running_CI = running_average(i)-(z_crit*running_error(i))
end

figure(1)
a1 = plot(1:simulations,running_average)
title('running average of estimators')
xlabel('simulations')
ylabel('sample mean')




wbar = sum(w)/simulations

% figure(2)
% a2 = errorbar(1:simulations,running_average,running_error)


% Problem 1.b


