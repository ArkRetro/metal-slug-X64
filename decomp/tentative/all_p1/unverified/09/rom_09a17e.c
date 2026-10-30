/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09a17e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9a17e(void)

{
  short sVar1;
  undefined2 uVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_120f8();
  candidate_9aad6();
  unaff_A6[0x14] = 0;
  candidate_4256();
  if ((*(byte *)((int)unaff_A6 + 0x86) & 1) == 0) {
    if (*(short *)(unaff_A6 + 0xf) < 0xc1) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0x70000;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
      *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfff90000;
    }
    uVar2 = 4;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    *(undefined4 *)((int)unaff_A6 + 0x62) = 0xfff00000;
    uVar2 = 5;
  }
  *(undefined2 *)((int)unaff_A6 + 0x7a) = uVar2;
  *unaff_A6 = 0x9a1fc;
  candidate_9ac86();
  candidate_427a();
  sVar1 = *(short *)((int)unaff_A6 + 0x7a);
  bVar3 = sVar1 == 0;
  sVar1 = sVar1 + -1;
  *(short *)((int)unaff_A6 + 0x7a) = sVar1;
  if (sVar1 == 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
    if (*(short *)(unaff_A6 + 0x1d) < 0) {
      *unaff_A6 = candidate_9a290;
      bVar3 = false;
    }
    else {
      *unaff_A6 = candidate_9a230;
      bVar3 = false;
    }
  }
  candidate_c8bec();
  if (bVar3) {
    *unaff_A6 = candidate_9a5ee;
  }
  return;
}
