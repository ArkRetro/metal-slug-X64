/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04677a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4677a(void)

{
  short sVar1;
  int extraout_A0;
  int *piVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_1abc8();
  for (piVar2 = (int *)0x473b0; *piVar2 != -1; piVar2 = piVar2 + 2) {
    bVar3 = false;
    candidate_40bc();
    if (bVar3) {
      *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)((int)unaff_A6 + 0x6e);
      *(undefined4 *)(extraout_A0 + 0x72) = *(undefined4 *)((int)unaff_A6 + 0x72);
      *(undefined1 *)(extraout_A0 + 0x80) = *(undefined1 *)(unaff_A6 + 0x20);
    }
    *(int *)(extraout_A0 + 0x86) = *piVar2;
    candidate_1acf2();
  }
  *(undefined4 *)((int)unaff_A6 + 0x86) = 0x47394;
  candidate_4681e();
  *unaff_A6 = 0x467e6;
  if (*(char *)((int)unaff_A6 + 0x8e) == '\0') {
    sVar1 = *(short *)(unaff_A6 + 0x23);
    bVar3 = sVar1 == 0;
    sVar1 = sVar1 + -1;
    *(short *)(unaff_A6 + 0x23) = sVar1;
    if (sVar1 < 0) goto LAB_00046804;
  }
  else {
    bVar3 = false;
    if (*(char *)((int)unaff_A6 + 0x8f) != '\0') {
LAB_00046804:
      candidate_4681e();
      if (bVar3) {
        *(undefined1 *)(unaff_A6[3] + 0x60) = 1;
        *unaff_A6 = 0x4681c;
      }
      return;
    }
  }
  return;
}
