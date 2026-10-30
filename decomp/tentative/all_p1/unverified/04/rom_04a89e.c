/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04a89e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4a89e(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(undefined1 *)((int)unaff_A6 + 0x9d) = 3;
  *(undefined1 *)((int)unaff_A6 + 0xa1) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x9e) = 0xff;
  iVar1 = *(int *)((int)unaff_A6 + 0x62);
  if (*(short *)(unaff_A6 + 10) < *(short *)((int)unaff_A6 + 0x8a)) {
    iVar1 = -iVar1;
  }
  unaff_A6[0xc] = iVar1;
  *unaff_A6 = 0x4a8ca;
  candidate_4d6b6();
  candidate_4e008();
  candidate_1ac38();
  candidate_427a();
  candidate_4e04e();
  if (*(char *)((int)unaff_A6 + 0x9e) == '\0') {
    if ((*(byte *)(unaff_A6 + 0x28) & 8) == 0) {
      if ((*(byte *)(unaff_A6 + 0x28) & 4) == 0) {
        *unaff_A6 = candidate_4a488;
      }
      else if ((*(byte *)(unaff_A6 + 0x28) & 2) == 0) {
        *unaff_A6 = 0x4a94c;
      }
      else {
        *unaff_A6 = candidate_4a5de;
      }
    }
    else {
      *unaff_A6 = 0x4a94c;
    }
  }
  bVar2 = false;
  candidate_4c24();
  if (bVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_4a9d4;
  }
  return;
}
