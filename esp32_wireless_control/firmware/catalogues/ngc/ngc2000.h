/**
 * @file ngc2000.h
 * @version 0.1.0
 *
 * @section License
 * Copyright (C) 2025, Sylensky
 */

#ifndef NGC2000_H
#define NGC2000_H

#include <Arduino.h>
#include <cstdint>
#include <vector>

#include "../star_database.h"

// Structure to hold NGC object data
struct NGCEntry
{
    String name;          // NGC or IC designation (e.g., "NGC1234", "IC456")
    String type;          // Object type as string (e.g., "Gx", "OC")
    float ra_hours;       // Right Ascension in hours (J2000)
    float dec_deg;        // Declination in degrees (J2000)
    String constellation; // Constellation abbreviation
    float size_arcmin;    // Largest dimension in arcminutes
    float magnitude;      // Integrated magnitude
    String notes;         // Object notes
    String description;   // Description field

    // Utility methods
    void print() const;
};

// Helper struct for binary NGC objects (compact format)
// Use pragma pack to ensure no padding and exact 83-byte size
#pragma pack(push, 1)
struct BinaryNGCEntry
{
    char id[12];           // e.g. "NGC104", "IC5340", null-padded
    char type[4];          // e.g. "Gx", "OC", "*", null-padded
    float ra;              // Right Ascension in hours
    float dec;             // Declination in degrees
    char constellation[3]; // 3 chars (no null terminator)
    float size_arcmin;
    float magnitude;
    char description[48]; // null-terminated or fixed
};
#pragma pack(pop)

// Main NGC2000 catalog class
// Implements DatabaseInterface
class NGC2000 : public StarDatabaseInterface
{
  public:
    NGC2000(const uint8_t* start, const uint8_t* end);
    NGC2000();
    virtual ~NGC2000();

    // DatabaseInterface implementations (binary only)
    bool loadDatabase(const char* binary_data, size_t len) override;
    bool isLoaded() const override;

    bool findByName(const String& name, StarUnifiedEntry& result) const override;
    bool findByNameFragment(const String& name_fragment, StarUnifiedEntry& result) const override;
    bool findByIndex(size_t index, StarUnifiedEntry& result) const override;

    void printDatabaseInfo() const override;
    size_t getTotalObjectCount() const override;

  private:
    const uint8_t* _start; // Start of raw data
    const uint8_t* _end;   // End of raw data
    size_t _object_count;
    std::vector<BinaryNGCEntry> _binary_entries;

    // Helper methods
    bool begin_binary(const uint8_t* data, size_t len);
    bool findNGCByName(const String& name, NGCEntry& result) const;
    bool findNGCByNameFragment(const String& name_fragment, NGCEntry& result) const;

    bool parseObjectFromBinary(const struct BinaryNGCEntry& bin, NGCEntry& obj) const;
    bool convertNGCToUnified(const NGCEntry& ngc, StarUnifiedEntry& unified) const;
};

extern NGC2000 ngc2000;

#endif // NGC2000_H
