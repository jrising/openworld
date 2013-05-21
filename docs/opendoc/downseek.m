close all

count = 4;
total = 100;
drawn = 20;
iters = 120;
% Output values-- matched by inputs 10, 20, 30, 40
outputs = log([10 20 30 40]);

input_vals = [ones(1, count) * total/count]; % #-samples x count
input_cnfs = [ones(1, count) * 0];

effective_means = ones(1, count) .* total / count;
effective_variances = ones(1, count) .* total / count;

for ii = 1:iters
  % Is it time to recalculate and redraw the graph?
  if mod(ii, drawn) == 0
    for jj = 1:count
      subplot(iters / drawn, count, count * (ii / drawn - 1) + jj);
      xx = 0:.1:50;
      yy = (1 / sqrt(2*pi*effective_variances(jj))) * ...
           exp(-((xx - effective_means(jj)).^2) / (2 * effective_variances(jj)));
      plot(xx, yy);
      hold on;
      plot(input_vals(ii-drawn+1:end, jj), .1*input_cnfs(ii-drawn+1:end, jj), '.');
    end

    % Determine mean and variance
    means = sum(input_vals .* input_cnfs) ./ sum(input_cnfs);
    means(isnan(means)) = total / count; %0;
    effective_means = (sum(input_cnfs) .* means + .1*total / count) ./ ...
        (sum(input_cnfs) + .1);
  
    variances = sum(input_cnfs .* (input_vals - ones(ii, 1) * means) ...
                    .^ 2) ./ (sum(input_cnfs) - max(input_cnfs));
    variances(isnan(variances)) = total / count; %0;
    effective_variances = (sum(input_cnfs) .* variances + .1*total / count) ./ ...
        (sum(input_cnfs) + .1);
  end
    
  % Select one to process
  select = ceil(count*rand(1));
  others = effective_means + sqrt(effective_variances) .* randn(1, count);
  others(others > total) = total / 2;
  others(others < 1) = 1;
  if sum(others) - others(select) > total - 1
      others = (total - 1) * others ./ (sum(others) - others(select));
  end
  inputs = [others(1:select-1) (total - (sum(others) - others(select))) others(select+1:end)];

  % Evalutate it!
  scores = (log(inputs) - log([10 20 30 40])) .^ 2;
  confs = exp(-scores * 100);
  input_vals = [input_vals; inputs];
  input_cnfs = [input_cnfs; confs];
end

% mean1 = sum1 / N = (sum0 + val) / N = (sum0 * (N - 1) + val) / N
% vari1 = (sum X - mean1)^2 / (N - 1) =? E X^2 - (EX)^2 = maybe...