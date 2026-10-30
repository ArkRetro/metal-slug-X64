/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ddae0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ddae0(void)

{
  undefined1 uVar2;
  undefined2 uVar1;
  byte bVar3;
  int extraout_A0;
  int unaff_A6;
  bool bVar4;

  if ((*(short *)(unaff_A6 + 0x7a) == 0) || (*(byte *)(unaff_A6 + 0xa2) < 2)) {
    if (*(short *)(unaff_A6 + 0x90) < 1) {
      if (*(char *)(unaff_A6 + 0x9d) == -1) {
        uVar2 = candidate_260c8();
        *(undefined1 *)(unaff_A6 + 0x9c) = uVar2;
        uVar2 = candidate_260c8();
        *(undefined1 *)(unaff_A6 + 0x9d) = uVar2;
      }
      if (*(char *)(unaff_A6 + 0x9c) < '\x01') {
        if (*(char *)(unaff_A6 + 0x9d) == '\0') {
          *(undefined1 *)(unaff_A6 + 0x9d) = 0xff;
          uVar1 = candidate_260c8();
          *(undefined2 *)(unaff_A6 + 0x90) = uVar1;
          *(char *)(unaff_A6 + 0xa2) = *(char *)(unaff_A6 + 0xa2) + '\x01';
        }
        else {
          bVar4 = false;
          candidate_40bc();
          if (bVar4) {
            candidate_1b220();
          }
          bVar4 = false;
          candidate_40bc();
          if (bVar4) {
            candidate_1b220();
          }
          uVar2 = candidate_260c8();
          *(undefined1 *)(unaff_A6 + 0x9c) = uVar2;
          *(char *)(unaff_A6 + 0x9d) = *(char *)(unaff_A6 + 0x9d) + -1;
        }
      }
      else {
        *(char *)(unaff_A6 + 0x9c) = *(char *)(unaff_A6 + 0x9c) + -1;
      }
    }
    else {
      *(short *)(unaff_A6 + 0x90) = *(short *)(unaff_A6 + 0x90) + -1;
    }
  }
  else {
    *(char *)(unaff_A6 + 0xa2) = *(char *)(unaff_A6 + 0xa2) + '\x01';
    if (0x10 < *(byte *)(unaff_A6 + 0xa2)) {
      *(undefined4 *)(unaff_A6 + 0x7e) = 0;
      bVar3 = 1;
      do {
        bVar4 = false;
        candidate_1aeb0();
        if ((bVar4) &&
           (*(short *)(extraout_A0 + 0x40) <= (short)(*(short *)(unaff_A6 + 0x7a) + 0x100))) {
          candidate_ddc90();
        }
        bVar3 = bVar3 - 1;
      } while (-1 < (short)((ushort)bVar3 << 8));
      if (*(int *)(unaff_A6 + 0x7e) != 0) {
        return;
      }
    }
  }
  return;
}
