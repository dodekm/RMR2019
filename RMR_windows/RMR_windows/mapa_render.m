%M=importdata('map.txt',',')
clear all;
close all;
%filename='floodfill.txt'
%filename='map.txt'
filename='path.txt'
%filename='file2.txt'
%filename='bludisko.txt'

cell_breakpoint=-3;
cell_path=-2;
cell_start =-1;
cell_free=0;
cell_obstacle=1;
cell_finish=2;

M=load(filename,'-ascii');
figure

imagesc(M)
hold on
grid on;
xlabel('X');
ylabel('Y');

[path_Y path_X]=find(M==cell_path);
plot(path_X, path_Y,'*k');

[breakpoint_Y breakpoint_X]=find(M==cell_breakpoint);
plot(breakpoint_X,breakpoint_Y,'*b');

[start_Y,start_X]=find(M==cell_start);
[finish_Y,finish_X]=find(M==cell_finish);
plot(start_X, start_Y,'*g');
plot(finish_X, finish_Y,'*r');
%%
dlmwrite('bludisko.txt',M,'delimiter',',','newline','pc')
%%
M=zeros(size(M));
%%
while(1)
imagesc(M);
hFH = imfreehand();
% Get the xy coordinates of where they drew.
[xy] = round(hFH.getPosition);
% get rid of imfreehand remnant.
delete(hFH);
paint_X=round(xy(:,1));
paint_Y=round(xy(:,2));
for i=1:length(paint_Y)
M(paint_Y(i),paint_X(i))=cell_obstacle;
end
imagesc(M);

end
%%
[lines,board,hist,result] = maze(50,50,[50,50],[0 0],0,0)
M=board;
spy(M)
