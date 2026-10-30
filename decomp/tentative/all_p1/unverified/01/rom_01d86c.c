/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01d86c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1d86c(void)

{
  undefined4 *unaff_A6;
  char cVar1;
  bool bVar2;
  char cVar3;
  undefined1 uVar4;

  candidate_1ad6e();
  *(ushort *)((int)unaff_A6 + 0x4e) = *(ushort *)(unaff_A6[3] + 0x4e) >> 1;
  candidate_4256();
  candidate_427a();
  *unaff_A6 = 0x1d8a4;
  cVar1 = '\0';
  bVar2 = false;
  cVar3 = '\0';
  uVar4 = 0;
  candidate_1dd90();
  if (bVar2 || cVar3 != cVar1) {
    *unaff_A6 = candidate_1d8d4;
    uVar4 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_4188;
    uVar4 = 0;
  }
  candidate_409a();
  if (!(bool)uVar4) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
