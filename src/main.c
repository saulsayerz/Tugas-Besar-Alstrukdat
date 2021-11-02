#include <stdio.h>
#include "../DaftarADT/map.c"
#include "../DaftarADT/pesanan.c"
#include "../DaftarADT/mobita.c"
#include "../DaftarADT/inventory.c"
#include "../DaftarADT/gadget.c"
#include "../DaftarADT/pcolor.c"
#include "../DaftarADT/ADTPrimitif/Queue.c"
#include "../DaftarADT/todo.c"
#include "../DaftarADT/node.c"

void move(Map peta, Queue *urutan, Mobita *player)
{
    printf("POSISI YANG DAPAT DICAPAI\n");
    int i, x, y, baris, kolom = 0, j, k;
    x = Absis(POSISI(*player)); // posisi sementara nobita
    y = Ordinat(POSISI(*player));
    ListDinamis dapatdicapai;
    CONTENTS(dapatdicapai) = (Lokasi *)malloc(26 * sizeof(Lokasi));
    NEFF(dapatdicapai) = 0;
    KAPASITAS(dapatdicapai) = 26;
    for (i = 0; i < NEFF(MAPLOC(peta)); i++)
    { // searching barisnya
        if (x == Absis(LOCPOINT(ELEMEN(MAPLOC(peta), i))) && y == Ordinat(LOCPOINT(ELEMEN(MAPLOC(peta), i))))
        {
            baris = i; // harusnya pake while deh algoritma searching tapi mager
        }
    }
    for (j = 0; j < NEFF(MAPLOC(peta)); j++)
    { // ngecek di matriks MAPADJ(peta) bisa dicapai ga
        if (ELEMENM(MAPADJ(peta), baris, j) == '1')
        {
            ELEMEN(dapatdicapai, kolom) = ELEMEN(MAPLOC(peta), j);
            kolom++;
            NEFF(dapatdicapai) += 1;
        }
    }
    for (k = 1; k <= NEFF(dapatdicapai); k++)
    {
        printf("%d. %c (%d,%d)\n", k, LOCNAME(ELEMEN(dapatdicapai, k - 1)), Absis(LOCPOINT(ELEMEN(dapatdicapai, k - 1))), Ordinat(LOCPOINT(ELEMEN(dapatdicapai, k - 1))));
    }
    printf("Posisi yang dipilih? (ketik 0 jika ingin kembali)\n");
    int pilihan;
    printf("ENTER COMMAND: ");
    scanf("%d", &pilihan);
    while (pilihan < 0 || pilihan > NEFF(dapatdicapai))
    {
        printf("Input salah! Masukkan ulang command\nCOMMAND: ");
        scanf("%d", &pilihan);
    }
    if (pilihan == 0)
    {
        printf("Kembali\n");
    }
    else
    {
        x = Absis(LOCPOINT(ELEMEN(dapatdicapai, pilihan - 1)));
        y = Ordinat(LOCPOINT(ELEMEN(dapatdicapai, pilihan - 1)));
        changePosisi(player, x, y);
        printf("Waktu : %d\n", WAKTU(*player));
        printf("Mobita sekarang berada di titik %c (%d,%d)!\n", LOCNAME(ELEMEN(dapatdicapai, pilihan - 1)), x, y);
    }
}

void pickUp(Map peta, Queue *urutan, Mobita *player)
{
    int i, x, y;
    x = Absis(POSISI(*player));
    y = Ordinat(POSISI(*player));

    char lokasiMobita;
    for (i = 0; i < NEFF(MAPLOC(peta)); i++)
    {
        if (x == Absis(LOCPOINT(ELEMEN(MAPLOC(peta), i))) && y == Ordinat(LOCPOINT(ELEMEN(MAPLOC(peta), i))))
        {
            lokasiMobita = LOCNAME(ELEMEN(MAPLOC(peta), i));
        }
    }
    int idx = 0;
    boolean exist = false;
    Address p = TODO(*player);
    printf("Nama lokasi mobita sekarang: %c\n", lokasiMobita);
    while (p != NULL && !exist)
    {
        if (p != NULL && lokasiMobita == ASAL(INFO(p)))
        {
            exist = true;
        }
        else
        {
            p = NEXT(p);
            idx++;
        }
    }

    if (exist)
    {
        printf("Pesanan berupa ");
        displayJenis(&INFO(p));
        printf(" berhasil diambil!!\n");
        printf("Tujuan Pesanan: %c\n", TUJUAN(INFO(p)));
        insertFirst(&INPROGRESS(*player), INFO(p));
        deleteAt(&TODO(*player), idx);
    }
    else
    {
        printf("Pesanan tidak ditemukan!");
    }
}

void dropOff(Map peta, Queue *urutan, Mobita *player)
{
    int i, x, y;
    x = Absis(POSISI(*player));
    y = Ordinat(POSISI(*player));

    char lokasiMobita;
    for (i = 0; i < NEFF(MAPLOC(peta)); i++)
    {
        if (x == Absis(LOCPOINT(ELEMEN(MAPLOC(peta), i))) && y == Ordinat(LOCPOINT(ELEMEN(MAPLOC(peta), i))))
        {
            lokasiMobita = LOCNAME(ELEMEN(MAPLOC(peta), i));
        }
    }
    Address p = INPROGRESS(*player);
    printf("Nama lokasi mobita sekarang: %c\n", lokasiMobita);
    if (p != NULL && lokasiMobita == TUJUAN(INFO(p)))
    {
        printf("Pesanan ");
        displayJenis(&INFO(p));
        printf(" berhasil diantarkan!\n");
        if (JENIS(INFO(p)) == 'N')
        {
            printf("Uang yang didapatkan : 200 Yen.");
            UANG(*player) += 200;
        }
        else if (JENIS(INFO(p)) == 'H')
        {
            printf("Uang yang didapatkan : 400 Yen.");
            UANG(*player) += 400;
        }
        else if (JENIS(INFO(p)) == 'P')
        {
            printf("Uang yang didapatkan : 400 Yen.");
            UANG(*player) += 400;
        }
        deleteFirst(&INPROGRESS(*player));
    }
    else
    {
        printf("Tidak ada pesanan yang dapat diantarkan!");
    }
    printf("\n");
}

void displayMapColor(Map peta, Queue *urutan, Mobita *player)
{ //INI BELUM JADIII
    Matrix m;
    ROWS(m) = MAPROW(peta) + 2;
    COLS(m) = MAPCOL(peta) + 2;
    Address p;
    int cetaknobita, cetaktodo, cetakprogress, cetakdicapai;
    int i, j, x, y,baris=0,kolom=0,k;
    x = Absis(POSISI(*player)); // posisi sementara nobita
    y = Ordinat(POSISI(*player));
    POINT titik = MakePOINT(0,0); 
    ListDinamis dapatdicapai;
    CONTENTS(dapatdicapai) = (Lokasi *)malloc(26 * sizeof(Lokasi));
    NEFF(dapatdicapai) = 0;
    KAPASITAS(dapatdicapai) = 26;
    for (i = 0; i < NEFF(MAPLOC(peta)); i++)
    { // searching barisnya
        if (x == Absis(LOCPOINT(ELEMEN(MAPLOC(peta), i))) && y == Ordinat(LOCPOINT(ELEMEN(MAPLOC(peta), i))))
        {
            baris = i; // harusnya pake while deh algoritma searching tapi mager
        }
    }
    for (j = 0; j < NEFF(MAPLOC(peta)); j++)
    { // ngecek di matriks MAPADJ(peta) bisa dicapai ga
        if (ELEMENM(MAPADJ(peta), baris, j) == '1')
        {
            ELEMEN(dapatdicapai, kolom) = ELEMEN(MAPLOC(peta), j);
            kolom++;
            NEFF(dapatdicapai) += 1;
        }
    }
    printf("%d\n", NEFF(dapatdicapai));
    for (i = 0; i < MAPROW(peta) + 2; i++)
    {
        for (j = 0; j < MAPCOL(peta) + 2; j++)
        {
            if (i == 0 || j == 0 || i == MAPROW(peta) + 1 || j == MAPCOL(peta) + 1)
            {
                ELEMENM(m, i, j) = '*';
            }
            else
            {
                ELEMENM(m, i, j) = ' ';
            }
        }
    }
    for (i = 0; i < NEFF(MAPLOC(peta)); i++)
    {
        x = Absis(LOCPOINT(ELEMEN(MAPLOC(peta), i)));
        y = Ordinat(LOCPOINT(ELEMEN(MAPLOC(peta), i)));
        ELEMENM(m, x, y) = LOCNAME(ELEMEN(MAPLOC(peta), i));
    }
    for (i = 0; i < ROWS(m); i++)
    {
        for (j = 0; j < COLS(m); j++)
        {
            cetaknobita = 0;
            cetaktodo = 0;
            cetakprogress = 0;
            cetakdicapai = 0;
            if (i == Absis(POSISI(*player)) && j == Ordinat(POSISI(*player))) //INI BERARTI LOKASI MOBITA (WARNA KUNING)
            {                                   
                print_yellow(ELEMENM(m, i, j)); // harusnya pake while deh algoritma searching tapi mager
                cetaknobita = 1;
            }
            if (!isEmpty(INPROGRESS(*player)) && !cetaknobita)
            {
                titik = NameToPoint(peta, TUJUAN(INFO(INPROGRESS(*player))));
                if (i== Absis(titik) && j==Ordinat(titik)) {
                    print_blue(ELEMENM(m, i, j));
                    cetakprogress = 1;
                }
            }
            if (!cetakprogress && !cetaknobita) 
            {
                p = TODO(*player);
                while (p != NULL) 
                {
                    titik = NameToPoint(peta, ASAL(INFO(p)));
                    if (i== Absis(titik) && j==Ordinat(titik)) 
                    {
                        cetaktodo = 1;
                    }
                    p = NEXT(p);
                }
                if (cetaktodo)
                {
                    print_red(ELEMENM(m, i, j));
                }
            }
            for (k = 0; k < NEFF(dapatdicapai); k++)
            {
                if (Absis(LOCPOINT(ELEMEN(dapatdicapai, k))) == i && Ordinat(LOCPOINT(ELEMEN(dapatdicapai, k))) == j)
                {
                    cetakdicapai = 1;
                    //TulisPOINT(LOCPOINT(ELEMEN(dapatdicapai,k)));
                }
            }    
            if (!cetakprogress && !cetaknobita && !cetaktodo && cetakdicapai)
            {
                print_green(ELEMENM(m, i, j));
            }
            if (!cetakprogress && !cetaknobita && !cetakdicapai && !cetaktodo)
            {
                printf("%c", ELEMENM(m, i, j));
            }
        }
        printf("\n");
    }
}

void buyGadget(Mobita *player, Gadget *gadget)
{
    int i;
    printf("\n");
    printf("Uang anda sekarang: %d Yen\n", UANG(*player));
    printf("Gadget yang tersedia: \n");
    for (i = 0; i < 5; i++)
    {
        printf("%d. ", (i + 1));
        displayName(NAMAGADGET(*(gadget + i)));
        printf("(%d Yen)", HARGAGADGET(*(gadget + i)));
        printf("\n");
    }
    printf("Gadget mana yang ingin kau beli? (ketik 0 jika ingin kembali)\n");
    printf("ENTER COMMAND: ");
    int choice;
    scanf("%d", &choice);
    if (choice)
    {
        Gadget wishlist = *(gadget + choice - 1);
        if (UANG(*player) < HARGAGADGET(wishlist))
        {
            printf("Uang tidak cukup untuk membeli gadget, silahkan top up.\n");
        }
        else
        {
            int success = addGadgetToInventory(&INVENTORY(*player), wishlist);
            if (success)
            {
                useUang(player, HARGAGADGET(wishlist));
                displayName(NAMAGADGET(wishlist));
                printf("berhasil dibeli!\n");
                printf("Uang anda sekarang: %d Yen", UANG(*player));
            }
            
        }
    }
    printf("\n");
}

void help()
{
    printf("1. MOVE\n");                // samuel udah
    printf("2. PICK UP\n");             // samuel udah
    printf("3. DROP OFF\n");            // samuel
    printf("4. DISPLAY MAP\n");         // saul
    printf("5. DISPLAY TO DO LIST\n");  // lewiss udah
    printf("6. DISPLAY IN PROGRESS\n"); // lewiss udah
    printf("7. BUY\n");                 //jova
    printf("8. INVENTORY\n");           //ave mungkin
    printf("9. HELP\n");                //udah
    printf("10. EXIT GAME\n");
    //harus ada yang ngeconvert Queue ke todolist dan inprogress berarti, mungkin diambil 5 6 juga soalnya mudah kan display doang
}

void pilihCommand(Map peta, Queue *urutan, Mobita *player, Gadget *gadget)
{
    int pilihan;
    printf("Silahkan pilih command: ");
    scanf("%d", &pilihan);
    if (pilihan == 1)
    {
        move(peta, urutan, player);
    }
    else if (pilihan == 2)
    {
        pickUp(peta, urutan, player);
    }
    else if (pilihan == 3)
    {
        dropOff(peta, urutan, player);
    }
    else if (pilihan == 4)
    {
        displayMapColor(peta, urutan, player);
    }
    else if (pilihan == 5)
    {
        displayTodo(TODO(*player));
    }
    else if (pilihan == 6)
    {
        displayInProgress(INPROGRESS(*player));
    }
    else if (pilihan == 7)
    {
        buyGadget(player, gadget);
    }
    else if (pilihan == 9)
    {
        help();
    }
    else
    {
        printf("Pilihan yang dimasukkan salah. Silahkan masukkan opsi lain\n");
    }

    int j = 0;
    while (WAKTUPESANAN(HEAD(*urutan)) <= WAKTU(*player))
    {
        ElType val;
        dequeue(urutan, &val);
        insertFirst(&TODO(*player), val);
    }

    pilihCommand(peta, urutan, player, gadget);
}

int main()
{
    int opsi, i;
    Map peta;
    DaftarPesanan daftar;
    Mobita player;
    Queue urutan;
    Gadget gadgets[5];
    CreateMap(&peta);
    printf("Selamat datang di game TUBES K2 Kelompok 6\nketik 1 untuk new game\nketik 2 untuk exit\nmasukkan opsi: ");
    scanf("%d", &opsi);
    if (opsi == 1)
    {
        konfigurasi(&peta, &daftar); // ngebaca txt file ke program
        printf("Konfigurasi permainan berhasil\n");
        createMobita(&player);
        sortPesanan(&daftar);
        CreateQueue(&urutan);
        for (i = 0; i < NEFF(daftar); i++)
        {
            enqueue(&urutan, ELEMEN(daftar, i));
        }
        int x = Absis(LOCPOINT(ELEMEN(MAPLOC(peta), 0))); // 0 adalah headquarter
        int y = Ordinat(LOCPOINT(ELEMEN(MAPLOC(peta), 0)));
        changePosisi(&player, x, y); // Mengubah koordinat awal nobita menjadi headquarter
        help();
        int j = 0;
        gadgets[0] = newGadget("Kain Pembungkus Waktu", 800);
        gadgets[1] = newGadget("Senter Pembesar", 1200);
        gadgets[2] = newGadget("Pintu Kemana Saja", 1500);
        gadgets[3] = newGadget("Mesin Waktu", 3000);
        gadgets[4] = newGadget("Senter Pengecil", 800);
        pilihCommand(peta, &urutan, &player, gadgets);
    }
    return 0;
}

// Queue [9 elemen], dequeue --> [8elemen]
// in progress sama to do list linked list [kosong] [kosong]
// to do list [1 elemen todolist] [kosong]
