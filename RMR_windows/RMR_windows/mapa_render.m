%M=importdata('map.txt',',')
clear all;
close all;
%filename='floodfill.txt'
%filename='map.txt'
filename='path.txt'

cell_breakpoint=-3;
cell_path=-2;
cell_start =-1;
cell_free=0;
cell_obstacle=1;
cell_finish=2;

M=load(filename,'ASCII');
M(M==cell_obstacle)=200;
figure

imshow(M)
hold on
grid on;
xlabel('X');
ylabel('Y');


[path_Y path_X]=find(M==cell_path);
plot(path_X, path_Y,'*k');

[breakpoint_Y breakpoint_X]=find(M==cell_breakpoint);
plot(breakpoint_X,breakpoint_Y,'*b');

[start_Y start_X]=find(M==cell_start);
[finish_Y finish_X]=find(M==cell_finish);
plot(start_X, start_Y,'*g');
plot(finish_X, finish_Y,'*r');



