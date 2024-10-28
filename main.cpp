#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

bool writeAffinityMask(int irq, const std::string &mask) {
    std::ostringstream path;
    path << "/proc/irq/" << irq << "/smp_affinity";

    std::ofstream affinityFile(path.str());
    if (!affinityFile.is_open()) {
        std::cerr << "Failed to open " << path.str() << " for IRQ " << irq << std::endl;
        return false;
    }

    affinityFile << mask;
    if (affinityFile.fail()) {
        std::cerr << "Failed to write affinity mask to " << path.str() << std::endl;
        return false;
    }

    affinityFile.close();
    std::cout << "Successfully set IRQ " << irq << " affinity mask to " << mask << std::endl;
    return true;
}

std::string calculateMask(int core) {
    if (core >= 32) {
        std::ostringstream mask;
        int idx = core / 32;
        int offset = core % 32;

        mask << std::hex << (1U << offset);
        for (int i = 1; i <= idx; ++i) {
            mask << ",00000000";
        }
        return mask.str();
    } else {
        std::ostringstream mask;
        mask << std::hex << (1U << core);
        return mask.str();
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " {all|local|remote|one|custom} [ethX]" << std::endl;
        std::cerr << "Author: Liansixin" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    std::string iface = argv[2];

    // Retrieve available CPU cores
    std::ifstream cpuOnlineFile("/sys/devices/system/cpu/online");
    std::string cores;
    if (cpuOnlineFile.is_open()) {
        std::getline(cpuOnlineFile, cores);
        cpuOnlineFile.close();
    } else {
        std::cerr << "Unable to read available CPU cores" << std::endl;
        return 1;
    }

    // Retrieve IRQs for the given network interface
    std::ostringstream irqCmd;
    irqCmd << "grep " << iface << " /proc/interrupts | awk '{print $1}' | sed 's/://'";

    FILE *cmd = popen(irqCmd.str().c_str(), "r");
    if (!cmd) {
        std::cerr << "Failed to retrieve IRQs for interface " << iface << std::endl;
        return 1;
    }

    std::vector<int> irqs;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), cmd)) {
        irqs.push_back(std::stoi(buffer));
    }
    pclose(cmd);

    if (irqs.empty()) {
        std::cerr << "No IRQs found for interface " << iface << std::endl;
        return 1;
    }

    int core = 0;
    for (int irq : irqs) {
        std::string mask = calculateMask(core);
        if (!writeAffinityMask(irq, mask)) {
            std::cerr << "Error setting affinity for IRQ " << irq << std::endl;
        }
        core++;
    }

    return 0;
}
