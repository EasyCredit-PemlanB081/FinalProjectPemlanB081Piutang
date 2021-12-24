#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
const char seperator[] = "____";
const char enter[] = "\n";

const int ID = 0;
const int ID_PIUTANG = 2;
const int NIK = 10;
const int NAMA = 11;
const int FLAG = 12;

//flag Lunas
const int LUNAS = 1;
const int BELUMLUNAS = 0;

const double denda = 0.1;

char filePiutang[] = "piutang.data";
char fileTagihan[] = "tagihan.data";
void printPiutang(int i);
void menuUtama();
void loadAllData();
//Fungsi
int getNow()
{
    return (int)time(NULL);
}

int isExistFile(char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "r");
    if (fp == NULL)
    {

        return 0;
    }
    else
    {
        fclose(fp);
        return 1;
    }
}

void makeFile(char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "w");
    fclose(fp);
}

char *readFileText(char *fileName)
{
    FILE *fp;
    char *text;
    int size;
    fp = fopen(fileName, "r");
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if (size == 0)
    {
        fclose(fp);
        return NULL;
    }
    else
    {
        fseek(fp, 0, SEEK_SET);
        text = (char *)malloc(sizeof(char) * size);
        fread(text, sizeof(char), size, fp);
        fclose(fp);
        return text;
    }
}

char *getDate(int timestamp)
{
    time_t epoch_time = timestamp;
    struct tm *converted_time;
    converted_time = localtime(&epoch_time);
    tzset();
    char *buffer = malloc(sizeof(char) * (26 + 1));
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", converted_time);
    return buffer;
}

typedef struct Piutang
{
    int timestamp;
    char *nama_pelanggan;
    char *nik;
    char *tanggal;
    double jumlahPiutang;
    double bunga;
    double sisaSaldo;
    double jumlahBayar;
    double sisaCicilan;
    double periode;
    char *klasifikasi;
} Piutang;

typedef struct Tagihan
{
    int timestamp;
    int timestamp_piutang;
    int timestamp_jatuhtempo;
    double jumlahCicilan;
    int cicilanKe;
    char *jatuhtempo;
    int flagbayar;
    Piutang piutang;
} Tagihan;

Tagihan *dataTagihan;
int sizeDataTagihan = 0;

Piutang *dataPiutang;
int sizeDataPiutang = 0;

void tukarPiutang(int i, int j)
{
    Piutang temp = dataPiutang[i];
    dataPiutang[i] = dataPiutang[j];
    dataPiutang[j] = temp;
}

void cekColumnPiutang(int i, int j, int column)
{
    if (column == ID)
    {
        if (dataPiutang[i].timestamp > dataPiutang[j].timestamp)
        {
            tukarPiutang(i, j);
        }
    }
    else if (column == NIK)
    {
        if (strcmp(dataPiutang[i].nik, dataPiutang[j].nik) > 0)
        {
            tukarPiutang(i, j);
        }
    }
    else if (column == NAMA)
    {
        if (strcmp(dataPiutang[i].nama_pelanggan, dataPiutang[j].nama_pelanggan) > 0)
        {
            tukarPiutang(i, j);
        }
    }
}

void sortingPiutang(int column)
{
    for (int i = 0; i < sizeDataPiutang; i++)
    {

        for (int j = i + 1; j < sizeDataPiutang; j++)
        {
            cekColumnPiutang(i, j, column);
        }
    }
}

void tukarTagihan(int i, int j)
{
    Tagihan temp = dataTagihan[i];
    dataTagihan[i] = dataTagihan[j];
    dataTagihan[j] = temp;
}

void cekColumnTagihan(int i, int j, int column)
{
    if (column == ID)
    {
        if (dataTagihan[i].timestamp > dataTagihan[j].timestamp)
        {
            tukarTagihan(i, j);
        }
    }
    else if (column == NIK)
    {
        if (strcmp(dataTagihan[i].piutang.nik, dataTagihan[j].piutang.nik) > 0)
        {
            tukarTagihan(i, j);
        }
    }
    else if (column == NAMA)
    {
        if (strcmp(dataTagihan[i].piutang.nama_pelanggan, dataTagihan[j].piutang.nama_pelanggan) > 0)
        {
            tukarTagihan(i, j);
        }
    }
    else if (column == ID_PIUTANG)
    {
        if (dataTagihan[i].timestamp_piutang > dataTagihan[j].timestamp_piutang)
        {
            tukarTagihan(i, j);
        }
    }
}

void sortingTagihan(int column)
{
    for (int i = 0; i < sizeDataTagihan; i++)
    {

        for (int j = i + 1; j < sizeDataTagihan; j++)
        {
            cekColumnTagihan(i, j, column);
        }
    }
}

int seachingTagihanByIDPiutang(int timestamp)
{
    if (sizeDataTagihan == 0)
    {
        return -1;
    }
    sortingTagihan(ID_PIUTANG);
    for (int i = 0; i < sizeDataTagihan; i++)
    {
        if (dataTagihan[i].timestamp_piutang == timestamp)
        {
            return i;
        }
    }
}

int searchingPiutangBelumLunasByNIK(char *nik)
{
    if (sizeDataPiutang == 0)
    {
        return -1;
    }
    sortingPiutang(NIK);

    int step = sizeDataPiutang == 1 ? 0 : sqrt(sizeDataPiutang);
    int jumper = step;
    int prev = 0;
    while (strcmp(dataPiutang[jumper].nik, nik) <= 0 && jumper < sizeDataPiutang)
    {

        //proses mencari batesan lompat
        prev = jumper;
        jumper += step;
        if (jumper >= sizeDataPiutang || prev == jumper)
        {
            break;
        }
    }

    int founded = 0;
    for (int i = prev; i <= jumper && i < sizeDataPiutang; i++)
    {

        if (strcmp(dataPiutang[i].nik, nik) == 0 && dataPiutang[i].sisaSaldo > 0)
        {
            return i;
        }
    }
    return -1;
}

int searchingPiutangBelumLunasByNama(char *nama_pelanggan)
{
    if (sizeDataPiutang == 0)
    {
        return -1;
    }
    sortingPiutang(NAMA);

    int step = sizeDataPiutang == 1 ? 0 : sqrt(sizeDataPiutang);
    int jumper = step;
    int prev = 0;
    while (strcmp(dataPiutang[jumper].nama_pelanggan, nama_pelanggan) <= 0 && jumper < sizeDataPiutang)
    {

        //proses mencari batesan lompat
        prev = jumper;
        jumper += step;
        if (jumper >= sizeDataPiutang || prev == jumper)
        {
            break;
        }
    }

    int founded = 0;
    for (int i = prev; i <= jumper && i < sizeDataPiutang; i++)
    {

        if (strcmp(dataPiutang[i].nama_pelanggan, nama_pelanggan) == 0 && dataPiutang[i].sisaSaldo > 0)
        {
            return i;
        }
    }
    return -1;
}

void setKlasifikasi(int index, int klasifikasi)
{

    if (klasifikasi <= 1)
    {
        dataPiutang[index].klasifikasi = "Lancar";
    }
    else if (klasifikasi <= 3)
    {
        dataPiutang[index].klasifikasi = "Dalam perhatian";
    }
    else if (klasifikasi <= 5)
    {
        dataPiutang[index].klasifikasi = "Kurang";
    }
    else if (klasifikasi < 7)
    {
        dataPiutang[index].klasifikasi = "Diragukan ";
    }
    else
    {
        dataPiutang[index].klasifikasi = "Macet ";
    }
}

void rewritePiutang()
{
    sortingPiutang(ID);

    FILE *fp;
    fp = fopen(filePiutang, "w");
    if (fp == NULL)
    {
        makeFile(filePiutang);
        fp = fopen(filePiutang, "w");
    }
    int size = sizeDataPiutang;
    for (int i = 0; i < size; i++)
    {
        fprintf(fp, "%d", dataPiutang[i].timestamp);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", dataPiutang[i].nik);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", dataPiutang[i].nama_pelanggan);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", dataPiutang[i].tanggal);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataPiutang[i].jumlahPiutang);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataPiutang[i].bunga);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataPiutang[i].sisaSaldo);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataPiutang[i].jumlahBayar);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataPiutang[i].sisaCicilan);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataPiutang[i].periode);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", dataPiutang[i].klasifikasi);
        fprintf(fp, "%s", enter);
    }

    fflush(fp);
    fclose(fp);
}

void rewriteTagihan()
{
    sortingTagihan(ID);

    FILE *fp;
    fp = fopen(fileTagihan, "w");
    if (fp == NULL)
    {
        makeFile(fileTagihan);
        fp = fopen(fileTagihan, "w");
    }
    int size = sizeDataTagihan;
    for (int i = 0; i < size; i++)
    {
        fprintf(fp, "%d", dataTagihan[i].timestamp);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", dataTagihan[i].timestamp_piutang);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", dataTagihan[i].timestamp_jatuhtempo);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", dataTagihan[i].jumlahCicilan);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", dataTagihan[i].cicilanKe);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", getDate(dataTagihan[i].timestamp_jatuhtempo));
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", dataTagihan[i].flagbayar);
        fprintf(fp, "%s", enter);
    }

    fflush(fp);
    fclose(fp);
}

void cetakTagihanBelumDibayarByIdPiutang(int index)
{
    system("cls");
    Piutang piutang = dataPiutang[index];
    int indexStart = seachingTagihanByIDPiutang(piutang.timestamp);
    printf("\t\t\tData Piutang\n");
    printPiutang(index);

    printf("\n\n=================================\n");
    for (int i = indexStart; i < indexStart + piutang.periode; i++)
    {
        if (dataTagihan[i].flagbayar == 0)
        {
            printf("\t\t\tCicilan ke %d\n", dataTagihan[i].cicilanKe);
            printf("\t\t\tJumlah Cicilan : %0.f\n", dataTagihan[i].jumlahCicilan);
            printf("\t\t\tJatuh tempo :%s\n\n", getDate(dataTagihan[i].timestamp_jatuhtempo));
        }
    }
}

void bayarCicilan(int index, int jumlahCicilan)
{
    int indexStart = seachingTagihanByIDPiutang(dataPiutang[index].timestamp);
    for (int i = indexStart; i < indexStart + dataPiutang[index].periode && jumlahCicilan > 0; i++)
    {
        if (dataTagihan[i].flagbayar == 0)
        {
            dataTagihan[i].flagbayar = 1;
            dataPiutang[index].sisaCicilan -= 1;
            dataPiutang[index].sisaSaldo -= dataTagihan[i].jumlahCicilan;
            dataPiutang[index].jumlahBayar += dataTagihan[i].jumlahCicilan;
            jumlahCicilan -= 1;
        }
    }
}

void lunasCicilan(int index)
{
    int indexStart = seachingTagihanByIDPiutang(dataPiutang[index].timestamp);
    for (int i = indexStart; i < indexStart + dataPiutang[index].periode; i++)
    {
        if (dataTagihan[i].flagbayar == 0)
        {
            dataTagihan[i].flagbayar = 1;
            dataPiutang[index].sisaCicilan -= 1;
            dataPiutang[index].jumlahBayar += dataTagihan[i].jumlahCicilan;
            dataPiutang[index].sisaSaldo -= dataTagihan[i].jumlahCicilan;
        }
    }
}

void bayarPiutang()
{
    system("cls");
    printf("\t\t\t********************************\n");
    printf("\t\t\t***  Bayar Tagihan Piutang   ***\n");
    printf("\t\t\t********************************\n");
    printf("\t\t\tMasukkan NIK Pelanggan : ");
    char nik[30];
    getchar();
    gets(nik);
    int index = searchingPiutangBelumLunasByNIK(nik);

    if (index > -1)
    {
        cetakTagihanBelumDibayarByIdPiutang(index);
        printf("\n\n\t\t\tPilih Metode Pembayaran");
        printf("\n\t\t\t1. 1 Cicilan");
        printf("\n\t\t\t2. Custom");
        printf("\n\t\t\t3. Lunas");

        printf("\n\n\t\t\tPilihan : ");
        int pilihan;
        scanf("%d", &pilihan);
        switch (pilihan)
        {
        case 1:
            bayarCicilan(index, 1);
            break;
        case 2:
            printf("\n\t\t\tMasukkan Jumlah Cicilan : ");
            int jumlahCicilan;
            scanf("%d", &jumlahCicilan);
            bayarCicilan(index, jumlahCicilan);

            printf("\n\t\t\tPembayaran berhasil ...\n");
            break;
        case 3:
            lunasCicilan(index);
            printf("\n\t\t\tPembayaran berhasil ...\n");
            break;
        default:
            printf("\n\t\t\tPilihan tidak tersedia\n");
            break;
        }
        rewritePiutang();
        rewriteTagihan();
        loadAllData();
        rewritePiutang();
        system("pause");
        menuUtama();
    }
    else
    {
        printf("\n\t\t\tMohon maaf, Tagihan tidak ditemukan\n");
        system("pause");
        menuUtama();
    }
}

void addPiutang(Piutang p, int write)
{
    // Masukkan dalam file
    if (write)
    {
        FILE *fp;
        fp = fopen(filePiutang, "a");
        if (fp == NULL)
        {
            makeFile(filePiutang);
            fp = fopen(filePiutang, "a");
        }

        fprintf(fp, "%d", p.timestamp);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", p.nik);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", p.nama_pelanggan);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", p.tanggal);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", p.jumlahPiutang);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", p.bunga);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", p.sisaSaldo);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", p.jumlahBayar);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", p.sisaCicilan);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", p.periode);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", p.klasifikasi);
        fprintf(fp, "%s", enter);
        fflush(fp);
        fclose(fp);
    }

    // tambahkan data di memori
    dataPiutang = (Piutang *)realloc(dataPiutang, sizeof(Piutang) * (sizeDataPiutang + 1));
    dataPiutang[sizeDataPiutang] = p;
    sizeDataPiutang++;
}

void addTagihan(Tagihan t)
{

    dataTagihan = (Tagihan *)realloc(dataTagihan, sizeof(Tagihan) * (sizeDataTagihan + 1));
    dataTagihan[sizeDataTagihan] = t;
    sizeDataTagihan++;
}

void generateTagihan(Piutang p)
{
    int timestampJatuhTempo = p.timestamp;
    for (int i = 0; i < p.periode; i++)
    {
        // Tambahkan 30 hari
        timestampJatuhTempo += (3600 * 24 * 30);
        Tagihan tagihan;
        tagihan.timestamp = getNow() + i;
        tagihan.timestamp_piutang = p.timestamp;
        tagihan.timestamp_jatuhtempo = timestampJatuhTempo;
        tagihan.cicilanKe = i + 1;
        tagihan.jumlahCicilan = p.jumlahPiutang * (100 + p.bunga) / 100 / p.periode;
        tagihan.flagbayar = 0;
        tagihan.piutang = p;

        FILE *fp;
        fp = fopen(fileTagihan, "a");
        if (fp == NULL)
        {
            makeFile(fileTagihan);
            fp = fopen(fileTagihan, "a");
        }

        fprintf(fp, "%d", tagihan.timestamp);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", tagihan.timestamp_piutang);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", tagihan.timestamp_jatuhtempo);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%0.f", tagihan.jumlahCicilan);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", tagihan.cicilanKe);
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%s", getDate(tagihan.timestamp_jatuhtempo));
        fprintf(fp, "%s", seperator);
        fprintf(fp, "%d", tagihan.flagbayar);
        fprintf(fp, "%s", enter);
        fflush(fp);
        fclose(fp);

        // Tambahkan data di memoria
        dataTagihan = (Tagihan *)realloc(dataTagihan, sizeof(Tagihan) * (sizeDataTagihan + 1));
        dataTagihan[sizeDataTagihan] = tagihan;
        sizeDataTagihan++;
    }
}

void printPiutang(int i)
{
    printf("\t\t\tNama Pelanggan :%s\n", dataPiutang[i].nama_pelanggan);
    printf("\t\t\tNIK :%s\n", dataPiutang[i].nik);
    printf("\t\t\tTanggal Piutang :%s\n", dataPiutang[i].tanggal);
    printf("\t\t\tDiskon:%0.f persen\n", dataPiutang[i].bunga);
    printf("\t\t\tJumlah Diterima :%0.f\n", dataPiutang[i].jumlahPiutang);
    printf("\t\t\tJumlah Bayar :%0.f\n", dataPiutang[i].jumlahBayar);
    printf("\t\t\tSisa Piutang :%0.f\n", dataPiutang[i].sisaSaldo);
    printf("\t\t\tStatus :%s\n\n", dataPiutang[i].klasifikasi);
}

void cariPiutang(int column)
{
    system("cls");
    if (column == NAMA)
    {
        printf("\t\t\tPencarian Piutang belum lunas berdasarkan Nama\n");
    }
    else
    {
        printf("\t\t\tPencarian Piutang belum lunas berdasarkan NIK\n");
    }
    fflush(stdin);
    char keyword[30];
    printf("\t\t\tMasukkan keyword pencarian : ");
    gets(keyword);
    int i;
    if (column == NAMA)
    {
        i = searchingPiutangBelumLunasByNama(keyword);
    }
    else
    {
        i = searchingPiutangBelumLunasByNIK(keyword);
    }

    if (i >= 0)
    {
        printPiutang(i);
    }
    else
    {
        printf("\t\t\tData tidak ditemukan\n");
    }
    system("pause");
    menuUtama();
}

void cariTagihan(int column)
{
    system("cls");
    if (column == NAMA)
    {
        printf("\t\t\tPencarian Tagihan belum lunas berdasarkan Nama\n");
    }
    else
    {
        printf("\t\t\tPencarian Tagihan belum lunas berdasarkan NIK\n");
    }
    fflush(stdin);
    char keyword[30];
    printf("\t\t\tMasukkan keyword pencarian : ");
    gets(keyword);
    int i;
    if (column == NAMA)
    {
        i = searchingPiutangBelumLunasByNama(keyword);
    }
    else
    {
        i = searchingPiutangBelumLunasByNIK(keyword);
    }

    if (i >= 0)
    {

        cetakTagihanBelumDibayarByIdPiutang(i);
    }
    else
    {
        printf("\t\t\tData tidak ditemukan\n");
    }
    system("pause");
    menuUtama();
}

void printAllPiutang()
{
    system("cls");
    printf("\n\t\t\t********************************");
    printf("\n\t\t\t***      Daftar Piutang      ***");
    printf("\n\t\t\t********************************\n");
    int size = sizeDataPiutang;
    for (int i = 0; i < size; i++)
    {
        printPiutang(i);
    }

    system("pause");
    menuUtama();
}

void printAllTagihan(int flag)
{
    system("cls");
    printf("\n\t\t\t********************************");
    printf("\n\t\t\t***      Daftar Tagihan      ***");
    printf("\n\t\t\t********************************\n");
    int size = sizeDataTagihan;
    for (int i = 0; i < size; i++)
    {
        if (dataTagihan[i].flagbayar == flag)
        {
            printf("Nama Pelanggan :%s\n", dataTagihan[i].piutang.nama_pelanggan);
            printf("Cicilan ke :%d\n", dataTagihan[i].cicilanKe);
            printf("Jumlah :%0.f\n", dataTagihan[i].jumlahCicilan);
            printf("Jatuh tempo :%s\n\n", getDate(dataTagihan[i].timestamp_jatuhtempo));
        }
    }

    system("pause");
    menuUtama();
}

void loadTabelPiutang()
{
    sizeDataPiutang = 0;
    if (isExistFile(filePiutang))
    {
        char *data = readFileText(filePiutang);
        //Check bila kosong
        if (data != NULL)
        {
            char *a, *c;
            for (a = strtok_r(data, enter, &c); a != NULL; a = strtok_r(NULL, enter, &c))
            {
                Piutang p;
                p.timestamp = atoi(strtok(a, seperator));
                p.nik = strtok(NULL, seperator);
                 if (p.nik == NULL)
                {
                    break;
                }
                p.nama_pelanggan = strtok(NULL, seperator);
                p.tanggal = strtok(NULL, seperator);
                p.jumlahPiutang = atof(strtok(NULL, seperator));
                p.bunga = atof(strtok(NULL, seperator));
                p.sisaSaldo = atof(strtok(NULL, seperator));
                p.jumlahBayar = atof(strtok(NULL, seperator));
                p.sisaCicilan = atof(strtok(NULL, seperator));
                p.periode = atof(strtok(NULL, seperator));
                p.klasifikasi = strtok(NULL, seperator);
                addPiutang(p, 0);
            }
        }
    }
    else
    {
        makeFile(filePiutang);
    }
}

void loadTabelTagihan()
{
    sizeDataTagihan = 0;
    if (isExistFile(fileTagihan))
    {
        char *data = readFileText(fileTagihan);

        //Check bila kosong
        if (data != NULL)
        {

            int idpiutang = 0;
            int klasifikasi = 0;
            char *a, *c;
            for (a = strtok_r(data, enter, &c); a != NULL; a = strtok_r(NULL, enter, &c))
            {

                Tagihan p;
                p.timestamp = atoi(strtok(a, seperator));
                p.timestamp_piutang = atoi(strtok(NULL, seperator));
                p.timestamp_jatuhtempo = atoi(strtok(NULL, seperator));
                p.jumlahCicilan = atof(strtok(NULL, seperator));
                p.cicilanKe = atoi(strtok(NULL, seperator));
                p.jatuhtempo = strtok(NULL, seperator);
                p.flagbayar = atoi(strtok(NULL, seperator));
                if (p.timestamp_piutang != dataPiutang[idpiutang].timestamp)
                {
                    idpiutang++;
                    klasifikasi = 0;
                }
                if (p.timestamp_jatuhtempo < getNow())
                {
                    klasifikasi++;
                }
                setKlasifikasi(idpiutang, klasifikasi);
                p.piutang = dataPiutang[idpiutang];
                addTagihan(p);
            }
        }
    }
    else
    {
        makeFile(fileTagihan);
    }
}

void deleteDataTagihan(int index)
{
    if (index < sizeDataTagihan)
    {
        for (int i = index; i < sizeDataTagihan - 1; i++)
        {
            dataTagihan[i] = dataTagihan[i + 1];
        }
        sizeDataTagihan--;
    }
}

void deleteDataPiutang(int index)
{
    if (index < sizeDataPiutang)
    {
        int indexStart = seachingTagihanByIDPiutang(dataPiutang[index].timestamp);
        printf("start :%d", indexStart);
        while (dataTagihan[indexStart].timestamp_piutang == dataPiutang[index].timestamp)
        {
            deleteDataTagihan(indexStart);
        }
        rewriteTagihan();
        for (int i = index; i < sizeDataPiutang - 1; i++)
        {
            dataPiutang[i] = dataPiutang[i + 1];
        }

        sizeDataPiutang--;
    }
}

void formPiutang()
{
    system("cls");
    printf("\n\t\t\t***********************************************");
    printf("\n\t\t\t****              Form Piutang             ****");
    printf("\n\t\t\t***********************************************");
    printf("\n\t\t\tMasukkan Pelanggan : ");
    char nama_pelanggan[30];
    getchar();
    gets(nama_pelanggan);
    printf("\t\t\tMasukkan NIK : ");
    char nik[30];
    gets(nik);
    double sisaUtang = 0;
    int recentUtang = searchingPiutangBelumLunasByNIK(nik);
    if (recentUtang > -1)
    {

        if (strcmp(dataPiutang[recentUtang].klasifikasi, "Lancar") != 0)
        {
            printf("\t\t\tPelanggan sudah memiliki piutang namun tidak lancar\n Silahkan inputkan ulang !!\n");
            system("pause");
            formPiutang();
        }
        sisaUtang = dataPiutang[recentUtang].sisaSaldo;
    }
    printf("\t\t\tMasukkan Jumlah Piutang : ");
    double jumlahPiutang = 0;
    scanf("%lf", &jumlahPiutang);
    printf("\t\t\tMasukkan Bunga : ");
    double bunga = 0;
    scanf("%lf", &bunga);
    printf("\t\t\tMasukkan Berapa bulan cicilan : ");
    double periode = 0;
    scanf("%lf", &periode);

    if (recentUtang > -1 && jumlahPiutang < sisaUtang)
    {
        printf("\t\t\tUtang terbaru harus melebihi saldo piutang\n");
        system("pause");
        formPiutang();
    }
    else if (bunga > 100)
    {
        printf("\t\t\tBunga tidak boleh lebih dari 100\n");
        system("pause");
        formPiutang();
    }
    else if (recentUtang > -1 && jumlahPiutang >= sisaUtang)
    {
        // Lunasi hutang sebelumnya
        lunasCicilan(recentUtang);
        rewritePiutang();
        rewriteTagihan();
    }

    Piutang p;
    p.timestamp = getNow();
    p.nama_pelanggan = nama_pelanggan;
    p.nik = nik;
    p.tanggal = getDate(p.timestamp);
    p.jumlahPiutang = jumlahPiutang - sisaUtang;
    p.bunga = bunga;
    p.sisaSaldo = jumlahPiutang * (100 + p.bunga) / 100;
    p.klasifikasi = "Lancar";
    p.jumlahBayar = 0;
    p.sisaCicilan = periode;
    p.periode = periode;
    addPiutang(p, 1);
    p.jumlahPiutang = jumlahPiutang;
    generateTagihan(p);
    printf("Data berhasil ditambah\n");
    system("pause");
    menuUtama();
}

void formEditPelanggan()
{
    system("cls");
    printf("\n\t\t\t********************************");
    printf("\n\t\t\t***       Edit Profil        ***");
    printf("\n\t\t\t********************************");
    printf("\n\t\t\tMasukkan NIK : ");
    char nik[30];
    getchar();
    gets(nik);

    int index = searchingPiutangBelumLunasByNIK(nik);

    if (index != -1)
    {
        printf("\t\t\tData lama :\n");
        printf("\t\t\tNama Pelanggan :%s\n", dataPiutang[index].nama_pelanggan);
        printf("\t\t\tNIK :%s\n\n", dataPiutang[index].nik);
        printf("\t\t\tData Baru :\n");
        printf("\t\t\tMasukkan Nama Pelanggan : ");
        gets(dataPiutang[index].nama_pelanggan);
        printf("\t\t\tMasukkan NIK Pelanggan : ");
        gets(dataPiutang[index].nik);
        rewritePiutang();
    }
    else
    {
        printf("\t\t\tData tidak ditemukan\n");
    }
    system("pause");
    menuUtama();
}

void formDeletePiutang()
{
    system("cls");
    printf("\n\t\t\t***********************************************");
    printf("\n\t\t\t**   Hapus Piutang (Akan menghapus tagihan)  **");
    printf("\n\t\t\t***********************************************");
    printf("\n\t\t\tMasukkan NIK : ");
    char nik[30];
    getchar();
    gets(nik);

    int index = searchingPiutangBelumLunasByNIK(nik);

    if (index != -1)
    {
        printf("\t\t\tData lama :\n");
        printf("\t\t\tNama Pelanggan :%s\n", dataPiutang[index].nama_pelanggan);
        printf("\t\t\tNIK :%s\n", dataPiutang[index].nik);
        printf("\t\t\tPiutang :%0.f\n", dataPiutang[index].jumlahPiutang);
        printf("\t\t\tApakah anda yakin untuk menghapus tekan (1) untuk melanjutkan ? :\n");
        int i = 0;
        scanf("%d", &i);
        if (i == 1)
        {
            printf("\t\t\tHapus data berhasil\n");
            deleteDataPiutang(index);
            rewritePiutang();
        }
    }
    else
    {
        printf("\t\t\tData tidak ditemukan\n");
    }
    system("pause");
    menuUtama();
}

void loadAllData()
{
    loadTabelPiutang();
    loadTabelTagihan();
}

void resetData()
{
    sizeDataPiutang = 0;
    sizeDataTagihan = 0;
    rewritePiutang();
    rewriteTagihan();
    loadAllData();
    menuUtama();
}

void menuTagihan()
{
    system("cls");
    printf("\n\t\t\t********************************");
    printf("\n\t\t\t***        Menu Tagihan      ***");
    printf("\n\t\t\t********************************");
    printf("\n\t\t\tPilih Menu : ");
    printf("\n\t\t\t1. Lihat Tagihan Sudah dibayar");
    printf("\n\t\t\t2. Lihat Tagihan Belum dibayar");
    printf("\n\t\t\t3. Cari Tagihan Belum dibayar berdasarkan NIK");
    printf("\n\t\t\t4. Cari Tagihan Belum dibayar berdasarkan NAMA");
    printf("\n\t\t\t5. Bayar Tagihan");
    printf("\n\t\t\t|| Tekan tombol lainnya untuk keluar ||\n");
    printf("\n\t\t\tMasukkan Pilihan : ");

    int pilih;
    scanf("%d", &pilih);
    switch (pilih)
    {
    case 1:
        printAllTagihan(LUNAS);
        break;
    case 2:
        printAllTagihan(BELUMLUNAS);
        break;
    case 3:
        cariTagihan(NIK);
        break;
    case 4:
        cariTagihan(NAMA);
        break;
    case 5:
        bayarPiutang();
        break;
    default:
        menuUtama();
        break;
    }
}

void menuPiutang()
{
    system("cls");
    printf("\n\t\t\t********************************");
    printf("\n\t\t\t***        Menu Piutang      ***");
    printf("\n\t\t\t********************************");
    printf("\n\t\t\tPilih Menu : ");
    printf("\n\t\t\t1. Lihat Piutang");
    printf("\n\t\t\t2. Tambah Piutang");
    printf("\n\t\t\t3. Edit Data Pelanggan");
    printf("\n\t\t\t4. Hapus Piutang");
    printf("\n\t\t\t5. Cari Piutang Belum Lunas (Berdasarkan NIK)");
    printf("\n\t\t\t6. Cari Piutang Belum Lunas (Berdasarkan Nama)");
    printf("\n\t\t\t|| Tekan tombol lainnya untuk keluar ||");
    printf("\n\t\t\tMasukkan Pilihan : ");

    int pilih;
    scanf("%d", &pilih);
    switch (pilih)
    {
    case 1:
        system("cls");
        printAllPiutang();
        break;
    case 2:
        system("cls");
        formPiutang();
        break;
    case 3:
        system("cls");
        formEditPelanggan();
        break;
    case 4:
        system("cls");
        formDeletePiutang();
        break;
    case 5:
        system("cls");
        cariPiutang(NIK);
        break;
    case 6:
        system("cls");
        cariPiutang(NAMA);
        break;
    default:
        menuUtama();
        break;
    }
}

void menuUtama()
{
    system("cls");
    printf("\t\t\t********************************\n");
    printf("\t\t\t****   Aplikasi Piutang     ****\n");
    printf("\t\t\t***    (Berlisensi OJK)      ***\n");
    printf("\t\t\t********************************\n");
    printf("\t\t\tPilih Menu Khusus: \n");
    printf("\t\t\t1. Data Piutang\n");
    printf("\t\t\t2. Data Tagihan\n");
    printf("\t\t\t3. Reset Data\n");
    printf("\t\t\t0. Exit\n");
    printf("\t\t\tMasukkan Pilihan Anda : ");

    int pilih;
    scanf("%d", &pilih);
    switch (pilih)
    {
    case 1:
        menuPiutang();
        break;
    case 2:
        menuTagihan();
        break;
    case 3:
        resetData();
        break;
    case 0:
        system("exit");
        system("cls");
        printf("Terima Kasih Telah Berkunjung Ke Aplikasi Kami");
        break;
    default:
        menuUtama();
        break;
    }
}

int main()
{
    loadAllData();
    menuUtama();
    return 0;
}