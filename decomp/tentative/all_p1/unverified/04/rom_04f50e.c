/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04f50e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4f50e(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar2;

  candidate_21288();
  if ((bool)in_CF) {
    *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0x7f;
    *unaff_A6 = candidate_4f5fe;
  }
  if (*(char *)((int)unaff_A6 + 0x61) == '\0') {
    *(short *)(unaff_A6 + 0x2a) = *(short *)(unaff_A6 + 0x2a) + -8;
  }
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0) {
    sVar1 = candidate_49e8();
    bVar2 = false;
    if (sVar1 == 0) {
      candidate_4c24();
      if (bVar2) {
        if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
          *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
          candidate_120f8();
          *unaff_A6 = candidate_1c706;
          goto LAB_0004f5f8;
        }
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      }
      if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) == 0) {
        candidate_4fa8e();
      }
      else {
        *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
        candidate_120f8();
        *unaff_A6 = candidate_1c706;
      }
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
      candidate_120f8();
      *unaff_A6 = candidate_1c706;
    }
  }
  else {
    *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
    candidate_120f8();
    *unaff_A6 = candidate_1c706;
  }
LAB_0004f5f8:
  candidate_4f8ac();
  return;
}
