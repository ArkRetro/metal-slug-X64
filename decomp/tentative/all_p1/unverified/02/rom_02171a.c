/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02171a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2171a(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int iVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar2;

  candidate_1abc8();
  candidate_1ad8c();
  candidate_427a();
  candidate_4c24();
  if ((bool)in_CF) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      bVar2 = false;
      candidate_40bc();
      iVar1 = extraout_A0;
      if (bVar2) {
        candidate_1b220();
        iVar1 = extraout_A0_00;
      }
      if ((*(byte *)((int)unaff_A6 + 0x5b) & 0x80) == 0) {
        *(byte *)(iVar1 + 0x15) = *(byte *)(iVar1 + 0x15) & 0xfe;
      }
      else {
        *(byte *)(iVar1 + 0x15) = *(byte *)(iVar1 + 0x15) | 1;
      }
      *unaff_A6 = candidate_21790;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  candidate_219b0();
  return;
}
