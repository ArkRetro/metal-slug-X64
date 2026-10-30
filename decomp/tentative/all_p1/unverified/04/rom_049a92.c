/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x049a92.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_49a92(void)

{
  int iVar1;
  byte bVar2;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0x30);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    if (iVar1 < -0xc000) {
      bVar2 = 3;
    }
    else if (iVar1 < -0x8000) {
      bVar2 = 2;
    }
    else if (iVar1 < -0x4000) {
      bVar2 = 1;
    }
    else {
      bVar2 = 0;
    }
  }
  else if (iVar1 < 0x4000) {
    bVar2 = 0;
  }
  else if (iVar1 < 0x8000) {
    bVar2 = 1;
  }
  else if (iVar1 < 0xc000) {
    bVar2 = 2;
  }
  else {
    bVar2 = 3;
  }
  if (*(byte *)(unaff_A6 + 0x96) < bVar2) {
    *(char *)(unaff_A6 + 0x96) = *(char *)(unaff_A6 + 0x96) + '\x01';
    return;
  }
  if (bVar2 < *(byte *)(unaff_A6 + 0x96)) {
    *(char *)(unaff_A6 + 0x96) = *(char *)(unaff_A6 + 0x96) + -1;
  }
  return;
}
