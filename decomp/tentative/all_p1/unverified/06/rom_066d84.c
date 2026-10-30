/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x066d84.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_66d84(void)

{
  int *piVar1;
  undefined4 *puVar2;
  byte bVar3;
  int unaff_A5;

  puVar2 = *(undefined4 **)(unaff_A5 + 0x4586);
  if (*(char *)(puVar2 + 1) == '\0') {
    return;
  }
  piVar1 = (int *)*puVar2;
  if ((*piVar1 != 0) && (candidate_3d22a(), *piVar1 != 0)) {
    candidate_3d25a();
  }
  bVar3 = 0;
  while (bVar3 < *(byte *)((int)puVar2 + 6)) {
    *(byte *)(puVar2 + 2) = bVar3;
    candidate_66ca4();
    bVar3 = *(char *)(puVar2 + 2) + 1;
  }
  return;
}
