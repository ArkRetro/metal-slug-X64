/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08d268.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

short candidate_8d268(void)

{
  undefined1 uVar2;
  short sVar1;
  bool bVar3;

  uVar2 = candidate_aaa88();
                    /* WARNING (jumptable): Sanity check requires truncation of jumptable */
                    /* WARNING: Could not find normalized switch variable to match jumptable */
  switch(uVar2) {
  case 1:
    return 8;
  case 2:
    return 10;
  case 3:
    return 0xc;
  case 4:
    return 0xd;
  case 5:
    return 0xe;
  case 6:
    return 0xf;
  case 7:
    return 0x11;
  case 8:
    return 0x13;
  case 9:
    return 0x14;
  case 10:
    return 0x15;
  case 0xb:
    return 0x16;
  case 0xc:
    return 0x17;
  }
  sVar1 = candidate_c05b8();
  bVar3 = false;
  candidate_1adba();
  if (bVar3) {
    return sVar1 + 4;
  }
  return sVar1;
}
