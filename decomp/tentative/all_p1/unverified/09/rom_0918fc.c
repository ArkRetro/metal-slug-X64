/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0918fc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Possible PIC construction at 0x0009193c: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x00091940) */
/* WARNING: Removing unreachable block (ram,0x00091944) */
/* WARNING: Removing unreachable block (ram,0x00091952) */

void candidate_918fc(void)

{
  int iVar1;
  int unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  if ((*(byte *)(unaff_A6 + 0x15) & 8) == 0) {
    return;
  }
  bVar2 = false;
  candidate_1aeb0();
  if (bVar2) {
    uVar3 = false;
    candidate_1aeb0();
    if ((bool)uVar3) {
      candidate_918e4();
      candidate_1af3c();
      if ((bool)uVar3) {
        candidate_1af3c();
        if ((bool)uVar3) {
          return;
        }
        iVar1 = 0x101574;
      }
      else {
        iVar1 = 0x1014c4;
      }
    }
    else {
      iVar1 = 0x1014c4;
    }
  }
  else {
    iVar1 = 0x101574;
  }
  if (*(short *)(unaff_A6 + 0x3c) < *(short *)(iVar1 + 0x3c)) {
    if (*(int *)(unaff_A6 + 0x62) < 0) {
      return;
    }
  }
  else if (-1 < *(int *)(unaff_A6 + 0x62)) {
    return;
  }
  return;
}
