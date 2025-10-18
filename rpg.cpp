#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <thread>
#include <limits>

// --- Generator Angka Acak ---
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

// --- Struktur & Enum ---

enum class CyberwareSlot { Optics, Arms, Deck };
enum class GigType { DataHeist, Sabotage, Extraction };

struct Player {
    std::string name;
    int health = 100;
    long long kreds = 500; // Mata uang di Neo-Kencana
    int streetCred = 0;
    int heat = 0;
    std::map<CyberwareSlot, int> cyberwareLevels; // Slot -> Level
};

struct Gig {
    std::string title;
    std::string description;
    GigType type;
    int difficulty; // 1-10
    int requiredCred;
    int rewardKreds;
    int rewardCred;
};

// --- Variabel Global ---
Player player;
std::vector<Gig> availableGigs;
bool isGameRunning = true;

// --- Fungsi Bantu ---
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void separator() { std::cout << "\n--------------------------------------------------\n"; }
void shortPause(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }
void pause() {
    std::cout << "\n(Tekan Enter untuk melanjutkan...)";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// --- Inisialisasi Game ---
void initializeGame() {
    player.cyberwareLevels[CyberwareSlot::Optics] = 1;
    player.cyberwareLevels[CyberwareSlot::Arms] = 1;
    player.cyberwareLevels[CyberwareSlot::Deck] = 1;

    availableGigs.push_back({"Curi Data Karyawan", "Infiltrasi server kecil milik Zentech dan curi daftar karyawan.", GigType::DataHeist, 2, 0, 800, 10});
    availableGigs.push_back({"Sabotase Gardu Listrik", "Rusak gardu listrik di sektor 3 untuk menyebabkan pemadaman.", GigType::Sabotage, 3, 5, 1200, 15});
    availableGigs.push_back({"Ekstraksi Jurnalis", "Bawa keluar seorang jurnalis dari apartemen yang diawasi Arasaka.", GigType::Extraction, 5, 20, 3000, 25});
    availableGigs.push_back({"Retas Jaringan Satelit", "Retas jaringan satelit milik Kyoran dan alihkan datanya.", GigType::DataHeist, 8, 50, 8000, 50});
    availableGigs.push_back({"Hancurkan Prototipe Mecha", "Masuk ke R&D Zentech dan hancurkan prototipe senjata terbaru mereka.", GigType::Sabotage, 10, 80, 15000, 100});
}

// --- Sistem Inti ---

void displayHUD() {
    std::cout << "MERC: " << player.name << " | Kreds: " << player.kreds << " | Street Cred: " << player.streetCred << " | Heat: " << player.heat << "\n";
    std::cout << "HEALTH: " << player.health << "/100\n";
    separator();
}

bool skillCheck(CyberwareSlot slot, int difficulty) {
    std::uniform_int_distribution<int> dist(1, 10);
    int playerRoll = player.cyberwareLevels[slot] + dist(rng);
    int targetRoll = difficulty + 5;
    
    std::cout << "Melakukan skill check... (Kekuatan: " << player.cyberwareLevels[slot] << " | Kesulitan: " << difficulty << ")\n";
    shortPause(1000);
    std::cout << "Hasil-mu: " << playerRoll << " vs. Target: " << targetRoll << "\n";
    shortPause(1000);

    return playerRoll >= targetRoll;
}

void executeGig(const Gig& gig) {
    clearScreen();
    std::cout << "--- MEMULAI GIG: " << gig.title << " ---\n";
    std::cout << gig.description << "\n";
    pause();

    bool success = false;
    if (gig.type == GigType::DataHeist) {
        std::cout << "Kamu harus meretas terminal utama. Ini akan menguji DECK-mu.\n";
        if (skillCheck(CyberwareSlot::Deck, gig.difficulty)) {
            std::cout << "BERHASIL! Firewall jebol, data berhasil dicuri.\n";
            success = true;
        } else {
            std::cout << "GAGAL! Sistem keamanan balik menyerang otakmu!\n";
            player.health -= 20;
        }
    } else if (gig.type == GigType::Sabotage) {
        std::cout << "Kamu harus membobol panel kontrol fisik. Ini menguji LENGAN-mu.\n";
        if (skillCheck(CyberwareSlot::Arms, gig.difficulty)) {
            std::cout << "BERHASIL! Panel hancur, target tersabotase.\n";
            success = true;
        } else {
            std::cout << "GAGAL! Kamu memicu alarm dan harus kabur!\n";
        }
    } else if (gig.type == GigType::Extraction) {
        std::cout << "Kamu harus menemukan target tanpa terdeteksi. Ini menguji MATA-mu.\n";
        if (skillCheck(CyberwareSlot::Optics, gig.difficulty)) {
            std::cout << "BERHASIL! Target ditemukan dan berhasil dievakuasi dengan senyap.\n";
            success = true;
        } else {
            std::cout << "GAGAL! Kamu salah jalan dan bertemu patroli keamanan!\n";
            player.health -= 15;
        }
    }

    if (success) {
        std::cout << "\nGig Selesai. Bayaran ditransfer.\n";
        std::cout << "+ " << gig.rewardKreds << " Kreds\n";
        std::cout << "+ " << gig.rewardCred << " Street Cred\n";
        player.kreds += gig.rewardKreds;
        player.streetCred += gig.rewardCred;
    } else {
        std::cout << "\nGig Gagal. Reputasimu tercoreng.\n";
        std::cout << "+5 Heat\n";
        player.heat += 5;
    }
    pause();
}

void checkGameStatus() {
    if (player.health <= 0) {
        clearScreen();
        std::cout << "Koneksi Saraf Terputus... Medis tidak merespon...\n";
        std::cout << "Kisahmu berakhir di gang basah Neo-Kencana.\n";
        std::cout << "--- GAME OVER ---\n";
        isGameRunning = false;
        pause();
    }
    if (player.streetCred >= 200) {
         clearScreen();
        std::cout << "Namamu kini menjadi legenda di jalanan Neo-Kencana.\n";
        std::cout << "Setiap fixer ingin merekrutmu, setiap korporasi takut padamu.\n";
        std::cout << "Kamu telah mencapai puncak rantai makanan di kota ini.\n";
        std::cout << "--- KAMU MENANG! ---\n";
        isGameRunning = false;
        pause();
    }
}

// --- Lokasi ---

void visitRipperdoc() {
    clearScreen();
    std::cout << "--- KLINIK RIPPERDOC ---\n";
    std::cout << "Ripperdoc menatapmu dengan mata sibernetiknya. \"Butuh upgrade, choom?\"\n";
    std::cout << "Kreds-mu: " << player.kreds << "\n";
    separator();
    std::cout << "Cyberware saat ini:\n";
    std::cout << "- Optics: Lv." << player.cyberwareLevels[CyberwareSlot::Optics] << "\n";
    std::cout << "- Arms: Lv." << player.cyberwareLevels[CyberwareSlot::Arms] << "\n";
    std::cout << "- Deck: Lv." << player.cyberwareLevels[CyberwareSlot::Deck] << "\n";
    separator();
    std::cout << "[1] Upgrade Optics (Biaya: " << 1000 * player.cyberwareLevels[CyberwareSlot::Optics] << " Kreds)\n";
    std::cout << "[2] Upgrade Arms (Biaya: " << 1000 * player.cyberwareLevels[CyberwareSlot::Arms] << " Kreds)\n";
    std::cout << "[3] Upgrade Deck (Biaya: " << 1000 * player.cyberwareLevels[CyberwareSlot::Deck] << " Kreds)\n";
    std::cout << "[4] Pulihkan Health (500 Kreds)\n";
    std::cout << "[5] Kembali ke jalanan\n> ";
    std::string choice;
    std::getline(std::cin, choice);

    long long cost = 0;
    CyberwareSlot slotToUpgrade;
    bool isUpgrade = false;

    if (choice == "1") { cost = 1000 * player.cyberwareLevels[CyberwareSlot::Optics]; slotToUpgrade = CyberwareSlot::Optics; isUpgrade = true;}
    else if (choice == "2") { cost = 1000 * player.cyberwareLevels[CyberwareSlot::Arms]; slotToUpgrade = CyberwareSlot::Arms; isUpgrade = true;}
    else if (choice == "3") { cost = 1000 * player.cyberwareLevels[CyberwareSlot::Deck]; slotToUpgrade = CyberwareSlot::Deck; isUpgrade = true;}
    else if (choice == "4") {
        if(player.kreds >= 500) {
            player.kreds -= 500;
            player.health = 100;
            std::cout << "Proses yang menyakitkan... tapi health-mu pulih sepenuhnya.\n";
        } else {
             std::cout << "Tidak cukup Kreds, choom.\n";
        }
    } else return;

    if (isUpgrade) {
        if (player.kreds >= cost) {
            player.kreds -= cost;
            player.cyberwareLevels[slotToUpgrade]++;
            std::cout << "Instalasi selesai. Kamu merasakan kekuatan baru mengalir.\n";
        } else {
            std::cout << "Dompetmu lebih tipis dari kesabaranku. Kembali kalau sudah punya uang.\n";
        }
    }
    pause();
}

void viewGigBoard() {
    clearScreen();
    std::cout << "--- GIG BOARD (Jaringan Gelap) ---\n";
    std::cout << "Pilih pekerjaan. Baca baik-baik, tidak ada pengembalian dana.\n";
    separator();
    
    std::vector<int> gigIndices;
    for (int i = 0; i < availableGigs.size(); ++i) {
        if (player.streetCred >= availableGigs[i].requiredCred) {
            std::cout << "[" << gigIndices.size() + 1 << "] " << availableGigs[i].title 
                      << " (Diff: " << availableGigs[i].difficulty 
                      << " | Bayaran: " << availableGigs[i].rewardKreds << ")\n";
            gigIndices.push_back(i);
        }
    }
    std::cout << "[" << gigIndices.size() + 1 << "] Kembali\n> ";
    
    int choice;
    std::cin >> choice;
    std::cin.get();

    if (choice > 0 && choice <= gigIndices.size()) {
        executeGig(availableGigs[gigIndices[choice - 1]]);
    }
}

void atSafehouse() {
    std::cout << "Flickering neon dari jalanan di bawah memantul di apartemenmu yang sempit. Ini adalah safehouse-mu.\n";
    std::cout << "[1] Cek Gig Board (Cari kerja)\n";
    std::cout << "[2] Kunjungi Ripperdoc (Upgrade & Pulih)\n";
    std::cout << "[3] Tunggu hingga Heat mereda (Lewati hari)\n";
    std::cout << "[4] Keluar dari Game\n> ";
    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "1") viewGigBoard();
    else if (choice == "2") visitRipperdoc();
    else if (choice == "3") {
        std::cout << "Kamu bersembunyi seharian. Situasi di jalanan sedikit lebih tenang.\n";
        if (player.heat > 0) player.heat--;
        pause();
    }
    else if (choice == "4") isGameRunning = false;
}

// --- Main Function ---
int main() {
    initializeGame();
    clearScreen();
    std::cout << "=======================================\n";
    std::cout << "|| CYBERPUNK MERCENARY: NEON SYNDICATE ||\n";
    std::cout << "=======================================\n";
    std::cout << "Hujan asam membasahi jalanan Neo-Kencana. Di dunia ini, kau bukan siapa-siapa tanpa reputasi.\n";
    std::cout << "Siapa namamu, merc?\n> ";
    std::getline(std::cin, player.name);

    while (isGameRunning) {
        clearScreen();
        displayHUD();
        atSafehouse();
        checkGameStatus();
    }

    std::cout << "Transmisi berakhir...\n";
    return 0;
}
