/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4422; tentative_c. */

/* WARNING: Removing unreachable block (ram,0x4897) */
/* WARNING: Removing unreachable block (ram,0x4744) */
/* WARNING: Removing unreachable block (ram,0x44ad) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte z80_candidate_4422(byte *param_1)

{
  short sVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  byte bVar5;
  undefined *puVar6;

  bVar2 = *_DAT_ram_fbfd;
  if (bVar2 != 0) {
    if (bVar2 != 1) {
      if (bVar2 != 2) {
        if (bVar2 != 3) {
          return bVar2;
        }
        puVar6 = &DAT_ram_fb9f;
        bVar2 = 0xff;
        bVar5 = 3;
        goto LAB_ram_49f1;
      }
      puVar6 = (undefined *)0x0;
      if (((_DAT_ram_faa9 == param_1) && (DAT_ram_fbf9 == cRamfab1)) && (DAT_ram_faae != '\0')) {
        puVar6 = &DAT_ram_faa9;
        bVar2 = 0;
        bVar5 = 3;
        goto LAB_ram_49f1;
      }
      bVar5 = 0;
      if (((param_1[9] & 2) == 0) || (bVar2 = 0, bVar4 = 0, (bRamfaac & 2) != 0)) {
        bVar3 = 1;
        if ((DAT_ram_faae == '\0') && (bVar3 = 2, DAT_ram_fab0 < DAT_ram_faaf)) {
          bVar3 = 3;
        }
        if ((bVar3 != 0) || (bVar2 = 0, bVar4 = 0, bVar3 == 0)) {
          puVar6 = &DAT_ram_faa9;
          bVar2 = DAT_ram_faad;
          bVar5 = bVar3;
          bVar4 = DAT_ram_faaf;
        }
      }
      if (((_DAT_ram_fad2 == param_1) && (DAT_ram_fbf9 == cRamfada)) && (DAT_ram_fad7 != '\0')) {
        puVar6 = &DAT_ram_fad2;
        bVar5 = 3;
        goto LAB_ram_49f1;
      }
      if (((param_1[9] & 2) == 0) || ((bRamfad5 & 2) != 0)) {
        bVar3 = 1;
        if ((DAT_ram_fad7 == '\0') && (bVar3 = 2, DAT_ram_fad9 < DAT_ram_fad8)) {
          bVar3 = 3;
        }
        if (bVar5 < bVar3) goto LAB_ram_4523;
        if (bVar5 == bVar3) {
          if (DAT_ram_fad6 == bVar2) {
            if (bVar4 <= DAT_ram_fad8) goto LAB_ram_4523;
          }
          else if (bVar2 <= DAT_ram_fad6) {
LAB_ram_4523:
            puVar6 = &DAT_ram_fad2;
            bVar2 = DAT_ram_fad6;
            bVar5 = bVar3;
            bVar4 = DAT_ram_fad8;
          }
        }
      }
      if (((_DAT_ram_fafb == param_1) && (DAT_ram_fbf9 == cRamfb03)) && (DAT_ram_fb00 != '\0')) {
        puVar6 = &DAT_ram_fafb;
        bVar5 = 3;
        goto LAB_ram_49f1;
      }
      if (((param_1[9] & 2) == 0) || ((bRamfafe & 2) != 0)) {
        bVar3 = 1;
        if ((DAT_ram_fb00 == '\0') && (bVar3 = 2, DAT_ram_fb02 < DAT_ram_fb01)) {
          bVar3 = 3;
        }
        if (bVar5 < bVar3) goto LAB_ram_458f;
        if (bVar5 == bVar3) {
          if (DAT_ram_faff == bVar2) {
            if (bVar4 <= DAT_ram_fb01) goto LAB_ram_458f;
          }
          else if (bVar2 <= DAT_ram_faff) {
LAB_ram_458f:
            puVar6 = &DAT_ram_fafb;
            bVar2 = DAT_ram_faff;
            bVar5 = bVar3;
            bVar4 = DAT_ram_fb01;
          }
        }
      }
      if (((_DAT_ram_fb24 == param_1) && (DAT_ram_fbf9 == cRamfb2c)) && (DAT_ram_fb29 != '\0')) {
        puVar6 = &DAT_ram_fb24;
        bVar5 = 3;
        goto LAB_ram_49f1;
      }
      if (((param_1[9] & 2) == 0) || ((bRamfb27 & 2) != 0)) {
        bVar3 = 1;
        if ((DAT_ram_fb29 == '\0') && (bVar3 = 2, DAT_ram_fb2b < DAT_ram_fb2a)) {
          bVar3 = 3;
        }
        if (bVar5 < bVar3) goto LAB_ram_45fb;
        if (bVar5 == bVar3) {
          if (DAT_ram_fb28 == bVar2) {
            if (bVar4 <= DAT_ram_fb2a) goto LAB_ram_45fb;
          }
          else if (bVar2 <= DAT_ram_fb28) {
LAB_ram_45fb:
            puVar6 = &DAT_ram_fb24;
            bVar2 = DAT_ram_fb28;
            bVar5 = bVar3;
            bVar4 = DAT_ram_fb2a;
          }
        }
      }
      if (((_DAT_ram_fb4d == param_1) && (DAT_ram_fbf9 == cRamfb55)) && (DAT_ram_fb52 != '\0')) {
        puVar6 = &DAT_ram_fb4d;
        bVar5 = 3;
        goto LAB_ram_49f1;
      }
      if (((param_1[9] & 2) == 0) || ((bRamfb50 & 2) != 0)) {
        bVar3 = 1;
        if ((DAT_ram_fb52 == '\0') && (bVar3 = 2, DAT_ram_fb54 < DAT_ram_fb53)) {
          bVar3 = 3;
        }
        if (bVar5 < bVar3) goto LAB_ram_4667;
        if (bVar5 == bVar3) {
          if (DAT_ram_fb51 == bVar2) {
            if (bVar4 <= DAT_ram_fb53) goto LAB_ram_4667;
          }
          else if (bVar2 <= DAT_ram_fb51) {
LAB_ram_4667:
            puVar6 = &DAT_ram_fb4d;
            bVar2 = DAT_ram_fb51;
            bVar5 = bVar3;
            bVar4 = DAT_ram_fb53;
          }
        }
      }
      if (((_DAT_ram_fb76 == param_1) && (DAT_ram_fbf9 == cRamfb7e)) && (DAT_ram_fb7b != '\0')) {
        puVar6 = &DAT_ram_fb76;
        bVar5 = 3;
      }
      else if (((param_1[9] & 2) == 0) || ((bRamfb79 & 2) != 0)) {
        bVar3 = 1;
        if ((DAT_ram_fb7b == '\0') && (bVar3 = 2, DAT_ram_fb7d < DAT_ram_fb7c)) {
          bVar3 = 3;
        }
        if (bVar3 <= bVar5) {
          if (bVar5 != bVar3) goto LAB_ram_49f1;
          if (DAT_ram_fb7a == bVar2) {
            if (DAT_ram_fb7c < bVar4) goto LAB_ram_49f1;
          }
          else if (DAT_ram_fb7a < bVar2) goto LAB_ram_49f1;
        }
        puVar6 = &DAT_ram_fb76;
        bVar2 = DAT_ram_fb7a;
        bVar5 = bVar3;
      }
      goto LAB_ram_49f1;
    }
    puVar6 = (undefined *)0x0;
    if (((_DAT_ram_f907 == param_1) && (DAT_ram_fbf9 == cRamf90f)) && (DAT_ram_f90c != '\0')) {
      puVar6 = &DAT_ram_f907;
      bVar2 = 0;
      bVar5 = 3;
      goto LAB_ram_49f1;
    }
    bVar5 = 0;
    if (((param_1[9] & 2) == 0) || (bVar2 = 0, bVar4 = 0, (bRamf90a & 2) != 0)) {
      bVar3 = 1;
      if ((DAT_ram_f90c == '\0') && (bVar3 = 2, DAT_ram_f90e < DAT_ram_f90d)) {
        bVar3 = 3;
      }
      if ((bVar3 != 0) || (bVar2 = 0, bVar4 = 0, bVar3 == 0)) {
        puVar6 = &DAT_ram_f907;
        bVar2 = DAT_ram_f90b;
        bVar5 = bVar3;
        bVar4 = DAT_ram_f90d;
      }
    }
    if (((_DAT_ram_f96f == param_1) && (DAT_ram_fbf9 == cRamf977)) && (DAT_ram_f974 != '\0')) {
      puVar6 = &DAT_ram_f96f;
      bVar5 = 3;
      goto LAB_ram_49f1;
    }
    if (((param_1[9] & 2) == 0) || ((bRamf972 & 2) != 0)) {
      bVar3 = 1;
      if ((DAT_ram_f974 == '\0') && (bVar3 = 2, DAT_ram_f976 < DAT_ram_f975)) {
        bVar3 = 3;
      }
      if (bVar5 < bVar3) goto LAB_ram_490d;
      if (bVar5 == bVar3) {
        if (DAT_ram_f973 == bVar2) {
          if (bVar4 <= DAT_ram_f975) goto LAB_ram_490d;
        }
        else if (bVar2 <= DAT_ram_f973) {
LAB_ram_490d:
          puVar6 = &DAT_ram_f96f;
          bVar2 = DAT_ram_f973;
          bVar5 = bVar3;
          bVar4 = DAT_ram_f975;
        }
      }
    }
    if (((_DAT_ram_f9d7 == param_1) && (DAT_ram_fbf9 == cRamf9df)) && (DAT_ram_f9dc != '\0')) {
      puVar6 = &DAT_ram_f9d7;
      bVar5 = 3;
      goto LAB_ram_49f1;
    }
    if (((param_1[9] & 2) == 0) || ((bRamf9da & 2) != 0)) {
      bVar3 = 1;
      if ((DAT_ram_f9dc == '\0') && (bVar3 = 2, DAT_ram_f9de < DAT_ram_f9dd)) {
        bVar3 = 3;
      }
      if (bVar5 < bVar3) goto LAB_ram_4979;
      if (bVar5 == bVar3) {
        if (DAT_ram_f9db == bVar2) {
          if (bVar4 <= DAT_ram_f9dd) goto LAB_ram_4979;
        }
        else if (bVar2 <= DAT_ram_f9db) {
LAB_ram_4979:
          puVar6 = &DAT_ram_f9d7;
          bVar2 = DAT_ram_f9db;
          bVar5 = bVar3;
          bVar4 = DAT_ram_f9dd;
        }
      }
    }
    if (((_DAT_ram_fa3f == param_1) && (DAT_ram_fbf9 == cRamfa47)) && (DAT_ram_fa44 != '\0')) {
      puVar6 = &DAT_ram_fa3f;
      bVar5 = 3;
    }
    else if (((param_1[9] & 2) == 0) || ((bRamfa42 & 2) != 0)) {
      bVar3 = 1;
      if ((DAT_ram_fa44 == '\0') && (bVar3 = 2, DAT_ram_fa46 < DAT_ram_fa45)) {
        bVar3 = 3;
      }
      if (bVar3 <= bVar5) {
        if (bVar5 != bVar3) goto LAB_ram_49f1;
        if (DAT_ram_fa43 == bVar2) {
          if (DAT_ram_fa45 < bVar4) goto LAB_ram_49f1;
        }
        else if (DAT_ram_fa43 < bVar2) goto LAB_ram_49f1;
      }
      puVar6 = &DAT_ram_fa3f;
      bVar2 = DAT_ram_fa43;
      bVar5 = bVar3;
    }
    goto LAB_ram_49f1;
  }
  puVar6 = (undefined *)0x0;
  if (((_DAT_ram_f81c == param_1) && (DAT_ram_fbf9 == cRamf824)) && (DAT_ram_f821 != '\0')) {
    puVar6 = &DAT_ram_f81c;
    bVar2 = 0;
    bVar5 = 3;
    goto LAB_ram_49f1;
  }
  bVar5 = 0;
  if (((param_1[9] & 2) == 0) || (bVar2 = 0, bVar4 = 0, (bRamf81f & 2) != 0)) {
    bVar3 = 1;
    if ((DAT_ram_f821 == '\0') && (bVar3 = 2, DAT_ram_f823 < DAT_ram_f822)) {
      bVar3 = 3;
    }
    if ((bVar3 != 0) || (bVar2 = 0, bVar4 = 0, bVar3 == 0)) {
      puVar6 = &DAT_ram_f81c;
      bVar2 = DAT_ram_f820;
      bVar5 = bVar3;
      bVar4 = DAT_ram_f822;
    }
  }
  if (((_DAT_ram_f869 == param_1) && (DAT_ram_fbf9 == cRamf871)) && (DAT_ram_f86e != '\0')) {
    puVar6 = &DAT_ram_f869;
    bVar5 = 3;
    goto LAB_ram_49f1;
  }
  if (((param_1[9] & 2) == 0) || ((bRamf86c & 2) != 0)) {
    bVar3 = 1;
    if ((DAT_ram_f86e == '\0') && (bVar3 = 2, DAT_ram_f870 < DAT_ram_f86f)) {
      bVar3 = 3;
    }
    if (bVar5 < bVar3) goto LAB_ram_47ba;
    if (bVar5 == bVar3) {
      if (DAT_ram_f86d == bVar2) {
        if (bVar4 <= DAT_ram_f86f) goto LAB_ram_47ba;
      }
      else if (bVar2 <= DAT_ram_f86d) {
LAB_ram_47ba:
        puVar6 = &DAT_ram_f869;
        bVar2 = DAT_ram_f86d;
        bVar5 = bVar3;
        bVar4 = DAT_ram_f86f;
      }
    }
  }
  if (((_DAT_ram_f8b6 == param_1) && (DAT_ram_fbf9 == cRamf8be)) && (DAT_ram_f8bb != '\0')) {
    puVar6 = &DAT_ram_f8b6;
    bVar5 = 3;
  }
  else if (((param_1[9] & 2) == 0) || ((bRamf8b9 & 2) != 0)) {
    bVar3 = 1;
    if ((DAT_ram_f8bb == '\0') && (bVar3 = 2, DAT_ram_f8bd < DAT_ram_f8bc)) {
      bVar3 = 3;
    }
    if (bVar3 <= bVar5) {
      if (bVar5 != bVar3) goto LAB_ram_49f1;
      if (DAT_ram_f8ba == bVar2) {
        if (DAT_ram_f8bc < bVar4) goto LAB_ram_49f1;
      }
      else if (DAT_ram_f8ba < bVar2) goto LAB_ram_49f1;
    }
    puVar6 = &DAT_ram_f8b6;
    bVar2 = DAT_ram_f8ba;
    bVar5 = bVar3;
  }
LAB_ram_49f1:
  if (bVar5 == 0) {
    return bVar5;
  }
  if (bVar5 < 2) {
    if (bVar2 < param_1[2]) {
      return bVar2;
    }
    puVar6[5] = 0;
  }
  bVar2 = *param_1;
  bVar5 = param_1[1];
  sVar1 = *(short *)param_1;
  *param_1 = (byte)puVar6;
  param_1[1] = (byte)((ushort)puVar6 >> 8);
  *puVar6 = (char)param_1;
  puVar6[1] = (char)((ushort)param_1 >> 8);
  if ((bVar2 | bVar5) != 0) {
    *(undefined1 *)(sVar1 + 4) = 0xff;
  }
  return bVar2 | bVar5;
}
