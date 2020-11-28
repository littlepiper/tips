% matlab cell convert .txt file
[nrows, ncols] = size(data);
fid = fopen("data.txt", 'w');
for row = 1:nrows
  fprintf(fid, '%s\n', data{row, :});
end
fclose(fid);
