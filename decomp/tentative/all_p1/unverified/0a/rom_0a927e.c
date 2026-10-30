/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a927e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a927e(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  unaff_A6[0x14] = 0xe64e0;
  candidate_4256();
  *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) & 0xfb;
  *unaff_A6 = 0xa92a2;
  uVar1 = 0;
  if (*(char *)((int)unaff_A6 + 0x95) == '\0') {
    candidate_1436a();
  }
  else {
    candidate_1abc8();
  }
  candidate_427a();
  if ((bool)uVar1) {
    candidate_a9b1c();
    if ((bool)uVar1) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
      unaff_A6[0x14] = 0;
      *unaff_A6 = candidate_a8fc2;
      uVar1 = 0;
    }
    else {
      candidate_a9b60();
      if ((bool)uVar1) {
        bVar2 = false;
        candidate_a9c42();
        if (bVar2) {
          *unaff_A6 = candidate_a915e;
          uVar1 = 0;
        }
        else {
          *unaff_A6 = candidate_a927e;
          uVar1 = 0;
        }
      }
      else {
        *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
        unaff_A6[0x14] = 0;
        *unaff_A6 = candidate_a8fc2;
        uVar1 = 0;
      }
    }
  }
  if ((*(byte *)(unaff_A6 + 0x18) & 4) != 0) {
    candidate_49e8();
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_a93dc;
  }
  candidate_a9a56();
  return;
}
