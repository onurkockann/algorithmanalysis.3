#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
	int n,winCount;
	float aP;//A nin kazanma olasiligini tutacak olan degiskenimiz.Boylelikle B nin kazanma olasiligi ise=(1-aP) olur.
	printf("Kac mac yapilacagini girin:");//kac mac yapilacagi kullanicidan isteniliyor.
	scanf("%d",&n);
	printf("A takiminin kazanma olasiligini(P) girin:");//A takimi icin esit durumlardaki bakilacak P(gelme olasiligi) degeri.1den buyuk deger girilmemesi gerekiyor.
	scanf("%f",&aP);
	
	if(aP>=0 && aP<=1){//olasilik degeri yalnizca 0-1 arasinda ise program calismaya devam eder.
		winCount=n/2 +1;
		int i,j;//matrisimizde satir ve sutun indisleri olarak kullanicagimiz degiskenlerimiz.
	
		float matrix[winCount+1][winCount+1];//matrisimizin satir ve sutun sayilari kazanmak icin gerekli olan mac sayisindan 1 fazla olmali.
		//Bunun nedeni ise kazanmak icin gerekli olan mac sayisinda 0 degerininde hesaplanmasidir.(Örnek:n=4 icin,0->1->2->3->4=5 indis)
	
		for(i=0;i<winCount;i++){
			matrix[i][0]=0;//B takiminin kazanmak icin kalan mac sayisi 0 oldugu durumlarda,A takiminin kazanma ihtimali 0dir.Matrisin bu gozlerini 0 olarak dolduruyoruz.
			matrix[0][i+1]=1;//A takiminin kazanmak icin kalan mac sayisi 1 oldugu durumlarda,B takiminin kaybetme(A takiminin kazanma)olasiligi 1dir.Satirlari bu sekilde dolduruyoruz.
		}
		//Elde ettigimiz baginti ile matrisin kalan yerlerini dolduruyoruz.1.satir ve 1.sutun komple dolduruldugu icin o hucreleri atliyoruz.
		for(i=1;i<=winCount;i++){
			for(j=1;j<=winCount;j++){//ilgili hucredeki kazanma olasiligini mevcut duruma gore hesaplamamiz icin ortaya cikardigimiz rekurans bagintimizi kullaniyoruz.
			//P(i,j) = p·P(i-1,j) + (1-p)·P(i,j-1)
				matrix[i][j]=(aP*matrix[i-1][j]) + ((1-aP)*matrix[i][j-1]);
			}
		}
	
		for(i=0;i<=winCount;i++){//olasiliklar tablosunu kullanicilarin gormesi icin matrisimizi yazdiriyoruz.
			for(j=0;j<=winCount;j++){
				printf("%.2f	",matrix[i][j]);
			}
			printf("\n");
		}
		printf("A takiminin kazanma olasiligi=%.2f",matrix[winCount][winCount]);//A takimi icin kazanma olasiligini yazdiriyoruz.
	}
	else
		printf("Olasilik degerini minimum 0,maximum 1 girmelisiniz.");

	
	return 0;
}
