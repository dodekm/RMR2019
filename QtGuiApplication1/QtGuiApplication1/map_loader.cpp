#include "map_loader.h"



 void map_loader::load_objects(char *filename,TMapArea &mapss)
 {

     FILE *fp=fopen(filename,"r");
     if (fp==NULL)
         return ;
     char myLine[550];
     fgets(myLine,550,fp);
     char *myCopy=(char*)calloc(strlen(myLine)+2,sizeof(char));
     memcpy(myCopy,myLine,sizeof(char)*strlen(myLine));
     char *freeMyCopy;
     freeMyCopy=myCopy;
     myCopy=strtok(myCopy,"[]");
     mapss.wall.numofpoints=(atoi(myCopy));
      mapss.wall.points.reserve(mapss.wall.numofpoints);
      for(int i=0;i<mapss.wall.numofpoints;i++)
      {
          Point temp;
          myCopy=strtok(NULL,"[,");
          temp.X=atof(myCopy);
          myCopy=strtok(NULL,"[,");
          temp.Y=atof(myCopy);
          mapss.wall.points.push_back(temp);
     

      }
      free(freeMyCopy);

       mapss.numofObjects=0;
        mapss.obstacle.clear();
      while( fgets(myLine,550,fp))
      {
  
          myCopy=(char*)calloc(strlen(myLine)+2,sizeof(char));
          memcpy(myCopy,myLine,sizeof(char)*strlen(myLine));

          freeMyCopy=myCopy;
          myCopy=strtok(myCopy,"[]");
          if((atoi(myCopy))==0)
              break;
          TMapObject tempObstacle;
          mapss.numofObjects++;

          tempObstacle.numofpoints=(atoi(myCopy));
          for(int i=0;i< tempObstacle.numofpoints;i++)
           {
              Point temp;
              myCopy=strtok(NULL,"[,");
              temp.X=atof(myCopy);
              myCopy=strtok(NULL,"[,");
              temp.Y=atof(myCopy);
              tempObstacle.points.push_back(temp);

           }
           free(freeMyCopy);
           mapss.obstacle.push_back(tempObstacle);
      }


 }
