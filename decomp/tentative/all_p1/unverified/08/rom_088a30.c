/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x088a30.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_88a30(void)

{
  undefined4 *unaff_A6;

  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0xaa) = 0;
  *unaff_A6 = 0x88a4a;
  candidate_4c24();
  candidate_146a8();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + 1;
  if (*(ushort *)(unaff_A6 + 0x26) <= *(ushort *)((int)unaff_A6 + 0x82)) {
    *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
    if (*(ushort *)(unaff_A6 + 0x20) < *(ushort *)((int)unaff_A6 + 0x96)) {
      *unaff_A6 = 0x888a8;
      if (0x128 < *(short *)(unaff_A6 + 0xf) || *(short *)(unaff_A6 + 0xf) < 0x18) {
        *(undefined2 *)((int)unaff_A6 + 0x7e) = 0;
        *unaff_A6 = 0x8830c;
      }
    }
    else {
      *(undefined2 *)((int)unaff_A6 + 0x7e) = 0;
      *unaff_A6 = 0x8830c;
    }
  }
  if (unaff_A6[0x28] != 0) {
    candidate_140c0();
  }
  candidate_89152();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  return;
}
