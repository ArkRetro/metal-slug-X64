/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x092934.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_92934(void)

{
  char cVar1;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int iVar2;
  int unaff_A6;
  bool bVar3;

  if (*(short *)(unaff_A6 + 0x8a) == 0) {
    bVar3 = false;
    candidate_aa956();
    if (bVar3) {
      *(undefined2 *)(unaff_A6 + 0x8a) = 0x18;
      candidate_40bc();
      candidate_1acf2();
      cVar1 = *(char *)(*(int *)(unaff_A6 + 0x70) + 5);
      *(short *)(extraout_A0 + 0x30) = (short)*(char *)(*(int *)(unaff_A6 + 0x70) + 4);
      *(short *)(extraout_A0 + 0x34) = (short)cVar1;
      *(undefined2 *)(extraout_A0 + 0x1e) = *(undefined2 *)(unaff_A6 + 0x1e);
      if (*(char *)(unaff_A6 + 0x7c) == '\x05') {
        candidate_40bc();
        iVar2 = extraout_A0_00;
      }
      else {
        candidate_40bc();
        iVar2 = extraout_A0_01;
      }
      *(undefined1 *)(iVar2 + 0x5e) = *(undefined1 *)(unaff_A6 + 0x5e);
      *(undefined2 *)(iVar2 + 0x6e) = *(undefined2 *)(unaff_A6 + 0x84);
      candidate_14a02();
      return;
    }
  }
  return;
}
