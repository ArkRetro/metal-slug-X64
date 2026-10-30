/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0424ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_424ac(void)

{
  int iVar1;
  int iVar2;
  char cVar3;
  int extraout_A0;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x4e) <= **(short **)(*(int *)(unaff_A6 + 0x72) + 0x78)) {
    candidate_411c();
    iVar1 = *(int *)(unaff_A6 + 0x72);
    iVar2 = *(int *)(iVar1 + 0x78);
    *(undefined4 *)(extraout_A0 + 0x86) = *(undefined4 *)(iVar2 + 2);
    *(int *)(iVar1 + 0x78) = iVar2 + 6;
    cVar3 = *(char *)(unaff_A6 + 0x85) + '\x01';
    *(char *)(unaff_A6 + 0x85) = cVar3;
    *(char *)(extraout_A0 + 0x85) = cVar3;
  }
  return;
}
