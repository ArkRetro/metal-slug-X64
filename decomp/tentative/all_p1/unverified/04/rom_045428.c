/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x045428.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_45428(void)

{
  char cVar1;
  int extraout_A0;
  int *piVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_1ad3e();
  candidate_146e8();
  for (piVar2 = (int *)0x45dbe; *piVar2 != -1; piVar2 = piVar2 + 2) {
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
  *(undefined4 *)((int)unaff_A6 + 0x86) = 0x45d84;
  candidate_45572();
  candidate_455ac();
  *unaff_A6 = 0x454ac;
  bVar3 = false;
  candidate_ca2f8();
  if (!bVar3) {
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 1;
    candidate_ca2e0();
  }
  bVar3 = false;
  candidate_ca2f8();
  if (!bVar3) {
    *(byte *)((int)unaff_A6 + 0x93) = *(byte *)((int)unaff_A6 + 0x93) | 2;
    candidate_ca2e0();
  }
  cVar1 = candidate_1ae8e();
  if (cVar1 == *(char *)((int)unaff_A6 + 0x93)) {
    *unaff_A6 = candidate_454fa;
  }
  return;
}
