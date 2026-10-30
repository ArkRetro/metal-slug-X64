/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a8e8e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a8e8e(void)

{
  char cVar1;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  unaff_A6[0x14] = 0;
  candidate_4256();
  candidate_1af9a();
  if (*(short *)(unaff_A6 + 0xf) < *(short *)(extraout_A0 + 0x3c)) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  *unaff_A6 = 0xa8ece;
  uVar2 = 0;
  if (*(char *)((int)unaff_A6 + 0x95) == '\0') {
    candidate_1436a();
  }
  else {
    candidate_1abc8();
  }
  if ((*(byte *)(unaff_A6 + 0x18) & 1) != 0) {
    uVar2 = 0;
    if (*(char *)((int)unaff_A6 + 0x93) == '\0') {
      uVar2 = 0;
      *(byte *)(unaff_A5 + 0x4606) = *(byte *)(unaff_A5 + 0x4606) ^ 0xff;
      *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) & 0xfe;
    }
    else if ((*(byte *)(unaff_A6 + 0x18) & 2) == 0) {
      *(byte *)(unaff_A5 + 0x4606) = *(byte *)(unaff_A5 + 0x4606) ^ 0xff;
      *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) | 2;
      uVar2 = *(short *)(unaff_A6 + 0x1d) != -1;
      if (*(short *)(unaff_A6 + 0x1d) != -1) {
        candidate_40bc();
        *(undefined4 *)(extraout_A0_00 + 0x6e) =
             *(undefined4 *)((short)(*(short *)(unaff_A6 + 0x1d) * 4) + 0x87a);
        *(undefined2 *)(extraout_A0_00 + 0x2c) = 0x100;
        uVar2 = 0;
      }
    }
  }
  candidate_427a();
  if ((bool)uVar2) {
    if (*(char *)((int)unaff_A6 + 0x93) == '\0') {
      cVar1 = candidate_19dca();
      if (cVar1 == '\0') {
        *unaff_A6 = candidate_a8fe0;
      }
      else {
        *unaff_A6 = candidate_a8df6;
      }
    }
    else if ((*(byte *)(unaff_A6 + 0x18) & 0x80) == 0) {
      *unaff_A6 = candidate_a8df6;
    }
    else {
      cVar1 = candidate_19dca();
      if (cVar1 == '\0') {
        *unaff_A6 = candidate_a8fe0;
      }
      else {
        *unaff_A6 = candidate_a8df6;
      }
    }
  }
  candidate_a9a56();
  return;
}
