#include<stdio.h>
#include<string.h>
#include<graphics.h>
#include<curl/curl.h>
#include<math.h>

size_t write_callback(void* received_data, size_t element_size, size_t element_count, char** first_data) {
    size_t total_size = element_size * element_count;
    *first_data = (char*)realloc(*first_data, total_size + 1);
    if (*first_data == NULL) {
        fprintf(stderr, "Bellek hatasý!\n");
        return 0;
    }
    memcpy(*first_data, received_data, total_size);
    (*first_data)[total_size] = '\0';
    return total_size;
}

void array_allocation(int coordinates[], int shape1[], int shape2[], int shape3[], int second, int third, int numberofCoordinates){
	if(second==0 && third==0){
		memcpy(shape1, coordinates, numberofCoordinates*sizeof(int));
	}
	else if(second!=0 && third==0){
		memcpy(shape1, coordinates, second*sizeof(int));
		memcpy(shape2, coordinates+second, (numberofCoordinates-second)*sizeof(int));
	}
	else{
		memcpy(shape1, coordinates, (second)*sizeof(int));
		memcpy(shape2, coordinates+second, (third-second)*sizeof(int));
		memcpy(shape3, coordinates+third, (numberofCoordinates-third)*sizeof(int));	
	}
	
}

void multiplied10(int array[], int array10[], int end){
	for(int i=0; i < end; i++){
		array10[i]= array[i] * 10;
	}
}

void draw_paint_areaofShapes(int coordinates[], int second, int third, int numberofCoordinates, int shape1[], int shape110[], int shape2[], int shape210[], int shape3[], int shape310[], float *reserveValue){
	setcolor(WHITE);
	for (int i = 0; i < numberofCoordinates -2 ; i+=2) {	
	    if(i!=0){
	    	if(coordinates[0]==coordinates[i] && coordinates[1]==coordinates[i+1]){
	    		second=i+2;
	    		break;
			}        
	    }
		line(coordinates[i] * 10, coordinates[i+1] * 10, coordinates[i+2] * 10, coordinates[i+3] * 10);
		*reserveValue += (coordinates[i] * coordinates[i + 3] - coordinates[i + 2] * coordinates[i+1]);
	}
		
	if(second != 0){
		for(int i=second;i<numberofCoordinates-2;i+=2){
			if(i!=second){
	    	    if(coordinates[second]==coordinates[i] && coordinates[second+1]==coordinates[i+1]){
	    	    	third=i+2;
	    		    break;
		    	}        
	        }
	    	line(coordinates[i] * 10, coordinates[i+1] * 10, coordinates[i+2] * 10, coordinates[i+3] * 10);	
	    	*reserveValue += (coordinates[i] * coordinates[i + 3] - coordinates[i + 2] * coordinates[i+1]);
    	}	
	}
	
	if(third != 0){
		for(int i=third;i<numberofCoordinates-2;i+=2){
			if(i!=third){
	    	    if(coordinates[third]==coordinates[i] && coordinates[third+1]==coordinates[i+1]){
	    	    	third=i+2;
	    		    break;
		    	}        
	        }
	    	line(coordinates[i] * 10, coordinates[i+1] * 10, coordinates[i+2] * 10, coordinates[i+3] * 10);	
	    	*reserveValue += (coordinates[i] * coordinates[i + 3] - coordinates[i + 2] * coordinates[i+1]);
    	}	
	}
	
	array_allocation(coordinates, shape1, shape2, shape3, second, third, numberofCoordinates);
	
    if (second == 0 && third == 0) {
    	multiplied10(shape1, shape110, numberofCoordinates);
    	
        setfillstyle(SOLID_FILL, RED);
        fillpoly(numberofCoordinates / 2, shape110);
    }
	else if (second != 0 && third == 0) {
		multiplied10(shape1, shape110, second);
		multiplied10(shape2, shape210, numberofCoordinates-second);
		
        setfillstyle(SOLID_FILL, RED);
        fillpoly((second)/ 2, shape110);
        fillpoly((numberofCoordinates - second) / 2, shape210);
    }
	else {
		multiplied10(shape1, shape110, second);
		multiplied10(shape2, shape210, third-second);
		multiplied10(shape3, shape310, numberofCoordinates-third);
		
		setfillstyle(SOLID_FILL, RED);
		fillpoly((second)/ 2, shape110);
		fillpoly((third-second) / 2, shape210);
		fillpoly((numberofCoordinates - third) / 2, shape310);	
    }	
}

int numberofRedPixels(int x, int y, int size) {
    int counter = 0;
    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            if (getpixel(i, j) == RED) {
                counter++;
            }
        }
    }
    return counter;
}


int main(void) {
    CURL* curl;
    CURLcode res;
    char* first_data=NULL;
    char* allocated_data=NULL;

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL,"http://bilgisayar.kocaeli.edu.tr/prolab1/prolab1.txt" ); 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &first_data);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "Curl hatasi: %s\n", curl_easy_strerror(res));
        } else {
            printf("Alinan veri:\n%s\n", first_data);
        }

        curl_easy_cleanup(curl);
    }
  
    int lineNumber;
    printf("Lutfen bir satir numarasi girin: ");
    scanf("%d", &lineNumber);

    allocated_data= strtok(first_data, "\n");
    for (int j = 1; j < lineNumber && allocated_data != NULL; ++j) {
        allocated_data = strtok(NULL, "\n");
    }

    if (allocated_data != NULL) {
        printf("Secilen Satir: %s\n", allocated_data);
    } else {
        printf("Secilen satir bulunamadi.\n");
    }	
    
    int coordinates[100];
    int numberofCoordinates= 0;
    int i = 0, x, y;
    int shape1[30]={0}, shape2[30]={0}, shape3[30]={0}, shape110[30]={0}, shape210[30]={0}, shape310[30]={0};
    float toplamAlan = 0.0;
    int second=0, third=0;
    int unitDrillingCost=0;
    int drillingArea=0;
    int numberofPlatforms=0;
    int unitPlatformCost=0;
    float reserveValue=0;
    int redPixels;

    while (allocated_data[i] != '\0') {
        if (allocated_data[i] == '(') {
            sscanf(allocated_data + i + 1, "%d,%d", &coordinates[numberofCoordinates], &coordinates[numberofCoordinates+1]);
            numberofCoordinates+=2;
        }
        i++;
    }
	
	int minx=coordinates[0];
	int miny=coordinates[1];
	int maxx=coordinates[0];
	int maxy=coordinates[1];
	
	for(int i=2;i<numberofCoordinates-2;i+=2){
		if(coordinates[i] <= minx){
			minx=coordinates[i];
		}
		if(coordinates[i+1] <= miny){
			miny=coordinates[i+1];
		}
		if(coordinates[i] >= maxx){
			maxx=coordinates[i];
		}
		if(coordinates[i+1] >= maxy){
			maxy=coordinates[i+1];
		}
	}
	
	do{
	printf("Birim sondaj maaliyetini giriniz(1 - 10): ");
    scanf("%d",&unitDrillingCost);
	}while(unitDrillingCost>10 || unitDrillingCost<1);
	
	printf("\Birim platform maliyetini giriniz: ");
	scanf("%d",&unitPlatformCost);
	

    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);
    
    setcolor(BLUE);
    for(x = 0; x < getmaxx(); x += 10) {
        for(y = 0; y < getmaxy(); y += 10) {
            rectangle(x, y, x + 10, y + 10);
        }
    }
     
    draw_paint_areaofShapes(coordinates, second, third, numberofCoordinates, shape1, shape110, shape2, shape210, shape3, shape310, &reserveValue);
    
	if(unitDrillingCost == 1 || unitDrillingCost == 2){ 
	    setfillstyle(SOLID_FILL, GREEN);
        setcolor(CYAN);
        for (int i = 160; i >= 10; i /= 2) {
            for (int x = minx*10; x < maxx*10; x += i) {
                for (int y = miny*10; y < maxy*10; y += i) {
                    redPixels = numberofRedPixels(x, y, i);
                    for(float j=1.0; j>0.4; j-=0.1){
                    	if (redPixels >= (i * i) * j && getpixel(x+(i/2),y+(i/2)) != GREEN) {
                            bar(x, y, x + i, y + i);
                            rectangle(x, y, x + i, y + i);
							numberofPlatforms++;
							drillingArea+=i*i;
                        }
					}
                    
                }
            }
        }  
    }
	
	else if(unitDrillingCost == 3 || unitDrillingCost == 4){
		setfillstyle(SOLID_FILL, GREEN);
        setcolor(CYAN);
		for (int i = 80; i >= 10; i /= 2) {
            for (int x = minx*10; x < maxx*10; x += i) {
                for (int y = miny*10; y < maxy*10; y += i) {
                    redPixels = numberofRedPixels(x, y, i);
                    for(float j=1.0; j>0.4; j-=0.1){
                    	if (redPixels >= (i * i) * j && getpixel(x+(i/2),y+(i/2)) != GREEN) {
                            bar(x, y, x + i, y + i);
                            rectangle(x, y, x + i, y + i);
							numberofPlatforms++;
							drillingArea+=i*i;
                        }
					}
                }
            }
        }  
	}
	else if(unitDrillingCost == 5 || unitDrillingCost == 6){
		setfillstyle(SOLID_FILL, GREEN);
        setcolor(CYAN);
		for (int i = 40; i >= 10; i /= 2) {
            for (int x = minx*10; x < maxx*10; x += i) {
                for (int y = miny*10; y < maxy*10; y += i) {
                    redPixels = numberofRedPixels(x, y, i);
                    for(float j=1.0; j>0.5; j-=0.1){
                    	if (redPixels >= (i * i) * j && getpixel(x+(i/2),y+(i/2)) != GREEN) {
                            bar(x, y, x + i, y + i);
                            rectangle(x, y, x + i, y + i);
							numberofPlatforms++;
							drillingArea+=i*i;
                        }
					}
                }
            }
        }
	}
	else if(unitDrillingCost == 7 || unitDrillingCost== 8){
		setfillstyle(SOLID_FILL, GREEN);
        setcolor(CYAN);
		for (int i = 20; i >= 10; i /= 2) {
            for (int x = minx*10; x < maxx*10; x += i) {
                for (int y = miny*10; y < maxy*10; y += i) {
                    redPixels = numberofRedPixels(x, y, i);
                    for(float j=1.0; j>0.5; j-=0.1){
                    	if (redPixels >= (i * i) * j && getpixel(x+(i/2),y+(i/2)) != GREEN) {
                            bar(x, y, x + i, y + i);
                            rectangle(x, y, x + i, y + i);
							numberofPlatforms++;
							drillingArea+=i*i;
                        }
					}
                }
            }
        }
	}
	else{ 
	    setfillstyle(SOLID_FILL, GREEN);
        setcolor(CYAN);
        for (int x = 0; x < getmaxx(); x += 10) {
            for (int y = 0; y < getmaxy(); y += 10) {
                redPixels = numberofRedPixels(x, y, 10);
                for(float j=1.0; j>0.7; j-=0.1){
                    if (redPixels >= (10 * 10) * j && getpixel(x+5,y+5) != GREEN) {
                        bar(x, y, x + 10, y + 10);
                        rectangle(x, y, x + 10, y + 10);
						numberofPlatforms++;
						drillingArea+=10*10;
                    }
				}
            }
        }
	}
    
    int tplatformCost=unitPlatformCost*numberofPlatforms;
    int tdrillingCost=unitDrillingCost*(drillingArea/100);
    int totalCost= tplatformCost+ tdrillingCost;
    float profitAmount = (fabs(reserveValue/2))*10 - totalCost;
    
    printf("\nToplam rezerv degeri: %.2f\n",(fabs(reserveValue/2))*10);
    printf("Toplam platform sayisi: %d tane\n", numberofPlatforms);
    printf("Toplam sondaj alani: %d\n", drillingArea/100);
    printf("Toplam platform maliyeti: %d\n",tplatformCost);
    printf("Toplam sondaj maliyeti: %d\n",tdrillingCost);
    printf("Toplam maliyet: %d\n",totalCost);
    printf("Kar miktari: %.2f\n",profitAmount);
	 
	getch();
    closegraph();
     
    return 0;
}

