/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01fc9e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1fc9e(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_1ac38();
  if ((*(byte *)(unaff_A6 + 0x23) & 1) == 0) {
    if (((0xef < *(short *)(unaff_A6 + 0x10)) && (0 < *(short *)(unaff_A6 + 0xf))) &&
       (*(char *)((int)unaff_A6 + 0x8b) == '\0')) {
      candidate_120f8();
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    }
  }
  else {
    if (((0x11f < *(short *)(unaff_A6 + 0x10)) && (0 < *(short *)(unaff_A6 + 0xf))) &&
       (*(char *)((int)unaff_A6 + 0x8b) == '\0')) {
      candidate_120f8();
      *(undefined1 *)((int)unaff_A6 + 0x8b) = 0xff;
    }
    if (((*(byte *)((int)unaff_A6 + 0x15) & 4) != 0) &&
       ((short)(*(short *)(unaff_A6[3] + 0x40) + 0x20) <= *(short *)(unaff_A6 + 0x10))) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfb;
    }
  }
  candidate_2047a();
  if (0x1ff < *(short *)(unaff_A6 + 0x10)) {
    *unaff_A6 = candidate_1fda4;
  }
  candidate_427a();
  candidate_140c0();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_1fece;
  }
  bVar2 = false;
  candidate_4c24();
  if (bVar2) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    else {
      *unaff_A6 = candidate_1fece;
    }
  }
  return;
}
