lambda = 1/2;
mu = 1/1;
frames = 10;
arriv_mean = 1/lambda;
process_mean = 1/mu;
simulations = 1000;
result_1 = zeros(simulations,frames);
transmission = zeros(simulations,frames);
arrivals = zeros(simulations,frames);

for i = 1:simulations
    for j = 1:frames
        arrivals(i,j) = exprnd(lambda);
        transmission(i,j) = exprnd(mu);
    end
end

% 
for i = 1:simulations
    queue = 0;
    prev_queue = 0;
    prev_trans = 0;
    for j = 1:frames
        t = transmission(i,j);
        a = arrivals(i,j);

        if j == 1                     
            queue = 0;
            a = 0;
        else
            % queuing delay = previous transmission + previous queuing
            % delay - new interarrival time
            queue = prev_trans + (prev_queue-a);
            
            %don't want negative delay
            if queue < 0
                queue = 0;
            end
        end
                    
        % sum queuing delay + transmission delay
        results_1(i,j) = queue + t;
        
        % Save results for next run
        prev_queue = queue;
        prev_trans = t;
    end
end
% 
% results of x runs
% Problem 1.a

% average of each simluation
w = sum(results_1,2);
% 
running_average = zeros(1,simulations);
upper_running_CI = zeros(1,simulations);
lower_running_CI = zeros(1,simulations);
lower_error = zeros(1,simulations);

rowwise_SD = std(results_1,0,2);

for i = 1:simulations
    running_average(i) = sum(w(1:i))/i;
end
 
wbar = sum(w)/simulations;
% 90 percent confidence interval

z_crit = 1.645;

rowwise_SD = std(results_1,0,2)

% Calculate the confidence interval at n simulations
for i = 1:simulations
    running_error(i) = rowwise_SD(i)/(sqrt(10));
    upper_running_CI = running_average(i)+(z_crit*running_error(i));
    lower_running_CI = running_average(i)-(z_crit*running_error(i));
end

disp(upper_running_CI)
disp(lower_running_CI)
graph_a = zeros(simulations,1)

% 
% 
% figure(2)
% a2 = errorbar(1:simulations,running_average,running_error);
% 

% Problem 1.b

zaverage = zeros(simulations,1);
Y = zeros(simulations,1);
Ybar = zeros(simulations,1);
EY = frames/mu;

for i = 1:simulations
    Y(i) = sum(transmission(i,:),2);
    Ybar(i) = sum(Y(1:i))/i;
end

for i = 1:simulations
    c = cov(running_average(1:i),Ybar(1:i))/var(Ybar(1:i));
    c = c(1,2);
    zaverage(i) = running_average(i) + (c*(Ybar(i)-EY));
end

% % Problem 1.c
Qbars = zeros(simulations,1);
Hbars = zeros(simulations,1);
Q = zeros(simulations,1);
EQ = (frames/mu) - (frames-1)/lambda;

for i = 1:simulations
    Q(i) = sum(transmission(i,:),2) - sum(arrivals(i,2:frames),2);
    Qbars(i) = sum(Q(1:i))/i;
end

for i = 1:simulations
    c = cov(running_average(1:i),Qbars(1:i))/var(Qbars(1:i));
    c = c(1,2);
    Hbars(i) = running_average(i) + (c * (Qbars(i) - EQ));
end

figure;
hold on;
title('running average of estimators');
xlabel('simulations');
ylabel('sample mean');

x_axis = (1:simulations-1);
plot(1:simulations,running_average,'DisplayName','part a');
plot(1:simulations,zaverage, 'DisplayName','Part b');
plot(1:simulations,Hbars, 'DisplayName','Part c');

legend('show');
hold off;

% figure;
% hold on;
% 
% hold off;