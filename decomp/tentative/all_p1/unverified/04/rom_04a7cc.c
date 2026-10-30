/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04a7cc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4a7cc(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  uVar2 = false;
  if (*(char *)((int)unaff_A6 + 0x9e) == '\0') {
    candidate_4ae98();
    if ((bool)uVar2) {
      *(byte *)((int)unaff_A6 + 0xa5) = *(byte *)((int)unaff_A6 + 0xa5) | 0x80;
    }
    uVar2 = 0;
    if (*(short *)(unaff_A6 + 0x22) == 0) {
      if ((*(byte *)((int)unaff_A6 + 0xa5) & 0x80) == 0) {
        if ((*(byte *)(unaff_A6 + 0x28) & 0x80) == 0) {
          if ((*(byte *)(unaff_A6 + 0x28) & 0x20) == 0) {
            *(byte *)(unaff_A6 + 0x28) = *(byte *)(unaff_A6 + 0x28) | 0x20;
            sVar1 = candidate_260c8();
            uVar2 = 0;
            if (sVar1 != 0) {
              candidate_4dc5c();
            }
          }
          else {
            *unaff_A6 = candidate_4a89e;
            uVar2 = 0;
          }
        }
        else {
          *(byte *)(unaff_A6 + 0x28) = *(byte *)(unaff_A6 + 0x28) & 0x7f;
          *(byte *)(unaff_A6 + 0x28) = *(byte *)(unaff_A6 + 0x28) | 0x20;
          candidate_4dc5c();
        }
      }
      else {
        *(byte *)(unaff_A6 + 0x28) = *(byte *)(unaff_A6 + 0x28) | 0x20;
        *(byte *)(unaff_A6 + 0x28) = *(byte *)(unaff_A6 + 0x28) & 0x7f;
        *(byte *)(unaff_A6 + 0x28) = *(byte *)(unaff_A6 + 0x28) | 8;
        *unaff_A6 = candidate_4a89e;
        uVar2 = 0;
      }
    }
    else {
      sVar1 = *(short *)(unaff_A6 + 0x22);
      uVar2 = sVar1 == 0;
      *(short *)(unaff_A6 + 0x22) = sVar1 + -1;
    }
  }
  candidate_4d6b6();
  candidate_4e008();
  candidate_1ac38();
  candidate_427a();
  candidate_4e04e();
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_4a9d4;
  }
  return;
}
