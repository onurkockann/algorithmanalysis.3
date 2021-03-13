#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M 999983

typedef struct kelimeVerileri{//tüm kelimelerimiz icin olusturdugumuz struct yapimiz
	int key;	//unique olusacak keyimiz
	char value[30];	//kelimenin kendisi
}kv;

typedef struct hataliKelimeVerileri{// tum hatali kelimelerimiz icin olusturdugumuz struct yapimiz
	int key;
	char value[30];	//asil (sozlukte bulunmayan)kelime
	char rValue[30];	//onerilen kelime
}hkv;

typedef struct onerilenKelimeler{//kelime onerimi esnasinda onerilen kelimeleri tutmak icin olusturdugumuz structimiz
	char kelime[30];	//kelimenin kendisi
	int key;	//unique degeri
	int distance;	//karsilastirildigi kelime ile olan uzakligi.
}recKelime;

recKelime aRec[M];	//onerilecek kelimelerin tutuldugu dizi.
kv a[M];	//kelimeleri tutacak M(1000 sayisina en yakýn asal sayý=997) boyutlu struct dizimiz.
hkv aNot[M]; //arama sonucu sozlukte olmayan kelimelerin tutuldugu dizi.

int h(int key,int i){//Istenýlen formuldeki hash fonksiyonumuz.
	int h1=key%M;
	int h2=1+(key%(M-1));
	return (h1+i*(h2))%M;
}

int insert (kv data){	//Hash tablosuna ekleme islemi.
    int i = 0;
    int keyGetir = h(data.key,i);	//keyGetir degiskeni ile kv tipinde data isimli verimizin keyi ve i degiskeni parametreleri ile hash key olusturulur.
    if(a[keyGetir].key==-1){//eger bu data hic eklenmemis ise
    	//tum degiskenleri ile olusan hash valuedeki indise ekle
        a[keyGetir].key=data.key;
        strcpy(a[keyGetir].value,data.value);
    }
    else{//eklenmemis ise
        while(a[keyGetir].key!=-1 && a[keyGetir].key!=data.key && i<M){//bos yer bulana kadar ve gelen deger mevcut indisdeki degere esit degilse iteratif bir sekilde ilerle ve
            i++;
            keyGetir = h(data.key,i);//en son olusturdugun keyi ve indisi tut
        }
        //bu indise elemaný ekle.
        a[keyGetir].key=data.key;
        strcpy(a[keyGetir].value,data.value);
    }
    
    
}
int insertWrong (hkv data){	//hatalý kelime tablosuna ekleme islemi.
    int i = 0;
    int keyGetir = h(data.key,i);	//keyGetir degiskeni ile kv tipinde data isimli verimizin keyi ve i degiskeni parametreleri ile hash key olusturulur.
    if(aNot[keyGetir].key==-1){//eger bu data hic eklenmemis ise
    	//tum degiskenleri ile olusan hash valuedeki indise ekle
        aNot[keyGetir].key=data.key;
        strcpy(aNot[keyGetir].value,data.value);
        strcpy(aNot[keyGetir].rValue,data.rValue);
        printf("(*)Onerdiginiz kelime hatali kelime tablosuna eklendi!Key=%d,Onerilen kelime=%s,asil kelime=%s\n==================\n",aNot[keyGetir].key,aNot[keyGetir].rValue,aNot[keyGetir].value);
    }
    else{//eklenmemis ise
        while(aNot[keyGetir].key!=-1 && aNot[keyGetir].key!=data.key && i<M){//bos yer bulana kadar ve gelen deger mevcut indisdeki degere esit degilse iteratif bir sekilde ilerle ve
            i++;
            keyGetir = h(data.key,i);//en son olusturdugun keyi ve indisi tut
        }
        //bu indise elemaný ekle.
        aNot[keyGetir].key=data.key;
        strcpy(aNot[keyGetir].value,data.value);
        strcpy(aNot[keyGetir].rValue,data.rValue);
        printf("(*)Onerdiginiz kelime hatali kelime tablosuna eklendi!\nKey:%d,Onerilen kelime=%s,asil kelime=%s\n==================\n",aNot[keyGetir].key,aNot[keyGetir].rValue,aNot[keyGetir].value);
    }
    
    
}

int LevenshteinED(char kelime[30],char sKelime[30]){//Levenshtein Edit Distance metodu ile iki kelimenin birbirine olan uzakligini bulan fonksiyon.
	int len1=strlen(kelime);//1.kelimenin uzunlugu
	int len2=strlen(sKelime);//2.(sozlukten sirayla gelen)kelimenin uzunlugu
    int matrix[len1 + 1][len2 + 1];//[0,0]boþ olan indis dahil uzunluklarin 1 fazlasi ile olusturdugumuz tablo icin tanimladigimiz matrisimiz.
    int diff = len1-len2;//bonus kismi icin farki tutmasý beklenen degiskenimiz.
    int i,j;//indisleri tutacak olan degiskenlerimiz.
    for (i = 0; i <= len1; i++) {//matrisin 0.sutunu dolduruluyor.
        matrix[i][0] = i;
    }
    for (i = 0; i <= len2; i++) {//matrisin 0.satiri dolduruluyor.
        matrix[0][i] = i;
    }
    for (i = 1; i <= len1; i++) {//matrisimizde hucreler geziliyor.
        char c1,c2;

        c1 = kelime[i-1];//kontrol icin ilk kelimedeki karakterler sirayla atilacak.
        for (j = 1; j <= len2; j++) {

            c2 = sKelime[j-1];//kontrol icin ikinci kelimedeki karakterler sirayla atilacak.
            if (c1 == c2) {//karakterler birbirine esit ise diagonelin bi ustunu bozma
                matrix[i][j] = matrix[i-1][j-1];
            }
            else {//esit degil ise
                int delete;
                int insert;
                int substitute;
                int minimum;

                delete = matrix[i-1][j] + 1;
                insert = matrix[i][j-1] + 1;
                substitute = matrix[i-1][j-1] + 1;
                minimum = delete;
                //minimumlarýný bulup +1 ekleyip ilgili matrix[i][j] indisine yazýyoruz
                if (insert < minimum) {
                    minimum = insert;
                }
                if (substitute < minimum) {
                    minimum = substitute;
                }
                matrix[i][j] = minimum;
                //BONUS kismi icin olan kontrol.
				if(i == (j + diff) && matrix[i][j] > 2){//burada onemli olan her zaman matrix[j][j+diff] huclerine bakiliyor olamasidir.eger bu hucrelerdeki degerlerden biri 2 den buyuk olursa
            			//kelimelerin uzakligi diagonelde 2 veya 2den buyuk gelir,daha dusuk mesafe gelemez.Boylelikle mesafeler kesin 2 den buyuk demektir
						//eger bu sart saglanirsa uzaklik 2den buyuk demektir -1 doner
						return -1;
					}
            }
        }
    }
    return matrix[len1][len2];//son satir ve son sutundaki bulunan deger mesafe degeridir.Bu deger dondurulur.
	
		
}

void searchWrong(char kelime[30],int key){//hatali kelime tablosunda arama yapan fonksiyonumuz.
    if (key != 0){//kelimenin sayi karsiligi olusturulmustur.Arama islemine gecilebilinir.
    	int i=0;
    	int keyGetir = h(key,i);//search yapmamiz icin keyGetir isimli degiskeni tekrar olusturuyoruz.
		if(aNot[keyGetir].key==key){//olusan keydeki hash valuemiz parametre olarak gelen key degerine esit ise tek adimda bulunmustur.Direkt ekrana bastir.
			printf("(*)Kelime hatali kelime tablosunda mevcut.Key:%d,Kelime:%s\n(*)Bu kelime icin onerilmis kelime=%s\n==================\n",aNot[keyGetir].key,aNot[keyGetir].value,aNot[keyGetir].rValue);
		}
		else{//kelime hatali kelime tablosunda yok ise;
			printf("(*)%s kelimesi hatali kelime tablosunda(Hash tablosunda) yok\n",kelime);
			//Levensthein Edit Distance uygulanir.
			int z,checkOne=0;//z degiskeni dongu icin,checkOne degiskeni ise mesafesi 1 olan kelimelerin var olup olmadigini tutmaya yariyan degiskenimiz.
			int dCounter=0;//onerilen kelime sayisini kontrol icin tutan degiskenimiz.
			for(z=0;z<M;z++){//tum diziyi gez
				if(a[z].key!=-1){//eleman olan indislere bak
					int dd=LevenshteinED(kelime,a[z].value);//mesafesini al.
					if(dd<=2 && dd!=-1){//mesafesi 2 veya 2den kucuk ise
						//onerilen kelimeler structina at.
						strcpy(aRec[dCounter].kelime,a[z].value);
						aRec[dCounter].key=1;
						aRec[dCounter].distance=dd;
						dCounter++;//onerilen kelime sayilarini tutan degiskenimiz arttirilir.
						if(dd==1)//mesafesi 1 olanlar mevcut ise.Switch degiskenimizi 1liyoruz ve boylelikle mesafesi 2 olanlar kullaniciya onerilmeyecek.
							checkOne=1;
					}
				}
			}
			if(dCounter==0)//onerilecek kelime hic bulunamadi.Cunku onerilen kelime sayisi=0
					printf("(*)Kelimenin sozlukteki tum kelimeler ile mesafesi 2den buyuk.Onerilecek kelime bulunamadi.\n==================\n");
			else{//onerilecek kelimeler onerildi ve kullanici isleme devam edicek.
				printf("(*)%s kelimesi icin onerilen kelimeler:\n",kelime,a[z].value);
				int k;//indis gorevi gormesi beklenen dongu degiskenimiz.
				for(k=0;k<dCounter;k++){//onerilen kelime sayisi kadar git.
					if(aRec[k].key==1 && aRec[k].distance==1){//once distance i 1 olan onerilen kelimeleri bastir.
						printf("%s Mesafesi->%d\n",aRec[k].kelime,aRec[k].distance);
					}
				}
				for(k=0;k<dCounter;k++){//distance i 1 olan kelime yok ise,distance i 2 olan kelimeleri bastir.
					if(aRec[k].key==1 && aRec[k].distance==2 && checkOne==0){//distance i 1 olanlar yok sadece 2 olan onerilen kelimeler.
						printf("%s Mesafesi->%d\n",aRec[k].kelime,aRec[k].distance);
					}
				}
				char newKelime[30];//kullanicinin onericegi yeni kelimeyi tutacak olan degiskenimiz.
				int chck=0;//kullanicinin onerilen kelimeler disinda farkli kelime girmesini engelliyecek olan degiskenimiz while icinde kullanilacaktir.
				while(chck==0){
					printf("(*)Lutfen %s kelimesi icin onerilen yeni kelimelerden birini yazin:",kelime);
					gets(newKelime);
					
					for(k=0;k<dCounter;k++){//sadece onerilen kelime sayisi kadar ilerle.
						if(strcmp(aRec[k].kelime,newKelime)==0)//kullanicinin girdigi kelime,onerilen kelimeler arasindami?Mevcut indisdeki kelime ile kiyasla.
							chck=1;//evet ise switchi 1le
					}
					if(chck==1){//kullanici onerilen kelimeler icinden bir kelime onerdiyse
						//hatali kelime verileri structi(hatali kelime tablosu icin olusturdugumuz struct)olusturulup;
						hkv data;
						strcpy(data.value,kelime);
						strcpy(data.rValue,newKelime);
						data.key=key;
						insertWrong(data);//hatali kelime tablosuna ekleme islemi yapan fonksiyonumuaz gonderilir.
					}
					else
						printf("(*)Lutfen yalnizca size onerilen kelimelerden birini girin.\n==================\n");
				}
				
			}
		}
                	
    }
}

void search(char kelime[30]){//kelimeyi sozlukte ariyan fonksiyonumuz
	//HORNER METOD ILE KELIMENIN SAYIYA CEVRILME ISLEMI:
    int length = strlen(kelime);
    int power = length-1;
    unsigned int key = 0;
    int i=0,j,k=0;
    char gecici;
    //kelimeyi kucuk harflere cevirme.
    int fark = 'a' - 'A';//ASCII table a gore once kucuk harfler sonra buyuk harfler oldugu icin fark 32 olur
    while (kelime[k] != '\0') {//NULL deger olmadikca devam et
        if (kelime[k] >= 'A' && kelime[k] <= 'Z') {//harf buyuk ise kucult.
            kelime[k] += fark;//fark kadar eklersek bu harfin ASCII degeri buyur ve ayný harfin kucugune sahip olmus olur
        }
        k++;
    }
    //kelimeyi kucultme islemi sonu.
    //HORNET METODUN ISLEM KISMI
    for(j=0;j<length;j++){
    	gecici = kelime[j];
    	if (gecici >= 'a')
    	   	gecici = 'A' - 'a' + gecici;
   		  	       	        
    	key += (pow(31,power)*(gecici-'A'+1));//us olarak 31(Asal sayi)verildi ve formul uygulandi.
    	power--;
    }
    key=abs(key);//tasmadan dolayi olusabilecek eksili key degerlerini onlemek adina mutlak degerini alip oyle key olusturuyoruz.
    if (key != 0){//kelimenin sayi karsiligi olusturulmustur.Arama islemine gecilebilinir.
    	int keyGetir = h(key,i);//search yapmamiz icin keyGetir isimli degiskeni olusturuyoruz.
		if(a[keyGetir].key==key){//olusan keydeki hash valuemiz parametre olarak gelen key degerine esit ise tek adimda bulunmustur.Direkt ekrana bastir.
			printf("(*)Kelime sozlukte mevcut=Key:%d,Kelime:%s\n==================\n",a[keyGetir].key,a[keyGetir].value);
		}
		else{//kelime sozlukte yok ise
			printf("(*)%s kelimesi sozlukte(Hash tablosunda) yok\n",kelime);
			searchWrong(kelime,key);//bir diger asama olan hatali kelime tablosunda arama asamasina gecilir.
		}
                	
    }
}

void searchSentence(char cumle[256]){//cumlenin tamamini alip kelime kelime ayirarak aratma fonksiyonuna gonderen fonksiyonumuz.
	int i=0,k=0,j;//k degiskeni kelime sonuna kadar dongunun gidilmesi icin kullanilan counter
	int kelimeSonu=strlen(cumle);//kelimenin sonunu tutmamiz icin olusturdugumuz degisken.Parametre olarak verilen cumlenin boyutunu tutar.
    char temp;//karakter kontrolu icin temp degiskenimiz
    char temp2[256];//cumle kontrolu icin temp2 degiskenimizi tanimladik.
    if(kelimeSonu==1)//tek harfli ise direkt aramaya sok.
    	search(cumle);
    else{
		while(k<=kelimeSonu){//cumle sonuna kadar git
       		temp = cumle[k];//harfi kontrol etmek icin al
       		if(temp == ' ' || k == kelimeSonu){//bosluk veya satir sonuna gelindi ise eger kelime olusmustur.
        		search(temp2);//olusan kelimeyi aramaya gonder
        		for(j=0;j<30;j++)//kelime tutan temp2 degiskenini temizle
        			temp2[j]='\0';
        		i=0;
        	}
        	else{
           		temp2[i] = temp;
           		i++;
        	}	
        	k++;
    	}
    }
}


void printArray(){//sozlukteki tum elemanlari ekrana bastiran fonksiyonumuz
	int i=0,j=0;
	for(i=0;i<M;i++){//dizi boyutu kadar ilerler
		if(a[i].key!=-1){//eger burda bir eleman var ise yazdirir.
			printf("Key:%d , Value:%s \n",a[i].key,a[i].value);
			j++;
		}
	}
	printf("Kelime adedi=%d\n",j);
}

void resetHashTable(){//hash tablelarin sifirlanmasi icin olusturulan fonksiyon.Hem sozluk hemde hatali kelime tablosu temizlenir.
	int i,j;
	for(i=0;i<M;i++)//dizi boyutu kadar ilerle
		a[i].key=-1;
	for(j=0;j<M;j++)
		aNot[j].key=-1;
}


int main(int argc, char *argv[]) {
	FILE* fp;
	fp = fopen("smallDictionary.txt","r");//kontrol icin bu dosya aciliyor
	if(fp == NULL){//dosya acilirken bir hata olusur ise
        printf("Dosya acilirken hata olustu!");//ekrana uyari bastir.
        fclose(fp);
   	}
   	else{//sorunsuz bir sekilde program acildi ise ana akisa gec;
		resetHashTable();//Hash tableimiz temizlniyor.
		
		//dosyadaki kelimeler tek tek okunur ve hash tablosuna aktarilir.
   		int j=0,i=0;//indis degiskenlerimiz
    	kv data;//kelime verileri olarak olusturdugumuz struct adina tanimladigimiz degiskenimiz.
    	unsigned int key=0;//horner metodu icin hesapladigimiz keyleri tutacak olan degiskenimiz.
    	char temp[50];//string kontrolu icin olusturdugumuz temp degiskenimiz.
    	char temp2[50];//stringimizde olusan 1 kelimeyi kontrol etmek icin olusturdugumuz temp degiskenimiz.
    	while(!feof(fp)){//dosya sonuna kadar git
        	temp[i] = fgetc(fp);//okudugun karakteri al.
        	if(temp[i] == ' ' || temp[i] == '\n'){//bosluk veya satir sonuna gelindi ise eger kelime olusmustur
        	    int length = strlen(temp2);
        	    //HORNET METODUN uygulanmasi.
            	int power = length-1;
            	key = 0;
            	data.key=0;
            	char temp3;
            	for(j=0;j<length;j++){//horner metod ile kelimenin sayiya donusturulmesi
            	    temp3 = temp2[j];
            	    if (temp3 >= 'a')
            	        temp3 = 'A' - 'a' + temp3;
            	        
            	    key += (pow(31,power)*(temp3-'A'+1));
            	    power--;
        	    }
        	    key=abs(key);//olusacak tasmalardan oturu negatif bir key degeri gelmemesi adina keyimizin mutlak degerini aliyoruz.
            	if (key != 0){//kelimemiz insert edilmeye hazir ve keyiyle birlikte elde edilmistir.Sozluk hash tablosuna insert islemi icin gonderilir.
            		data.key=key;
            		strcpy(data.value,temp2);
                	insert(data);
                	
            	}
            	for(j=0;j<50;j++){//kelimeleri olusturduktan sonra temp gorevine devam etmesi icin temp2 degiskenimizi sifirliyoruz.
                	temp2[j] = '\0';
                	data.value[j]='\0';
            	}
            	i=0;
	
        	}
        	else{//yukardaki sart saglanmiyorsa kelime okunmaya devam ediyordur.
            	temp2[i] = temp[i];
            	i++;
        	}
    	}
    	fclose(fp);//dosyayi kapat.
    	printArray();//ve sozlugu yazdir
    	
    	while(1){
    	char cumle[256];
    	printf("==================BASLANGIC==================\n(1)Lutfen aramak istediginiz cumleyi girin:");
		gets(cumle);
		searchSentence(cumle);//kullanicidan girilen cumleyi,cumle arama fonksiyonuna gonder.
		}
		
   	}
	
	return 0;
}
