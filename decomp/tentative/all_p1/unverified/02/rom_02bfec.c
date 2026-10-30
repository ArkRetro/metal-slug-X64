/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02bfec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2bfec(void)

{
  ushort uVar1;
  short sVar2;
  int extraout_A0;
  int unaff_A6;

  *(short *)(unaff_A6 + 0x82) = *(short *)(unaff_A6 + 0x82) + -1;
  if (*(short *)(unaff_A6 + 0x82) < 1) {
    uVar1 = candidate_29c6();
    *(ushort *)(unaff_A6 + 0x82) = (uVar1 & 0x1f) + 0x30;
    *(char *)(unaff_A6 + 0x8a) = *(char *)(unaff_A6 + 0x8a) + '\x01';
    if (*(char *)(unaff_A6 + 0x8a) < '\x02') {
      uVar1 = candidate_29c6();
      sVar2 = (uVar1 & 0xf) << 2;
      *(undefined2 *)(unaff_A6 + 0x7a) = *(undefined2 *)(sVar2 + 0x2c54a);
      *(undefined2 *)(unaff_A6 + 0x7c) = *(undefined2 *)(sVar2 + 0x2c54c);
      uVar1 = candidate_29c6();
      *(ushort *)(unaff_A6 + 0x62) = (uVar1 & 0x1f0) + 0x100;
    }
    else {
      candidate_1af9a();
      *(undefined2 *)(unaff_A6 + 0x7a) = *(undefined2 *)(extraout_A0 + 0x3c);
      *(undefined2 *)(unaff_A6 + 0x7c) = *(undefined2 *)(extraout_A0 + 0x40);
      *(undefined2 *)(unaff_A6 + 0x62) = 0x300;
      if ('\x03' < *(char *)(unaff_A6 + 0x8a)) {
        return;
      }
    }
    uVar1 = candidate_29c6();
    *(short *)(unaff_A6 + 0x7a) = (uVar1 & 0x1f) + *(short *)(unaff_A6 + 0x7a);
    uVar1 = candidate_29c6();
    *(short *)(unaff_A6 + 0x7c) = (uVar1 & 0x1f) + *(short *)(unaff_A6 + 0x7c);
  }
  return;
}
