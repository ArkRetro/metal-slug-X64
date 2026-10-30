/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03ec3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3ec3a(void)

{
  int extraout_A0;
  undefined4 *unaff_A6;

  if (*(short *)((int)unaff_A6 + 0x82) != 0) {
    candidate_40bc();
    candidate_1acf2();
    *(ushort *)(extraout_A0 + 0x6e) = (ushort)((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0);
    *(undefined2 *)(extraout_A0 + 0x70) = 0x300;
    *(undefined2 *)(extraout_A0 + 0x72) = *(undefined2 *)((int)unaff_A6 + 0x72);
    *unaff_A6 = 0x3ec88;
  }
  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  candidate_3fada();
  return;
}
