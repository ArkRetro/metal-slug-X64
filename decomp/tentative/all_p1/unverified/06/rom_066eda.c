/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x066eda.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_66eda(void)

{
  int *piVar1;
  int unaff_A5;

  piVar1 = *(int **)(unaff_A5 + 0x4586);
  if ((piVar1 != *(int **)(unaff_A5 + 0x4582)) &&
     (*(char *)((int)piVar1 + 7) != *(char *)(piVar1 + 2))) {
    return;
  }
  if (((*(ushort *)(*piVar1 + 0xe) & 0xe) == 2) &&
     (*(char *)((int)piVar1 + 7) != *(char *)(piVar1 + 2))) {
    return;
  }
  return;
}
