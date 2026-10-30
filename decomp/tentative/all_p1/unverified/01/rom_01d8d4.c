/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01d8d4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1d8d4(void)

{
  undefined4 *unaff_A6;
  char cVar1;
  bool bVar2;
  char cVar3;
  undefined1 uVar4;

  *(ushort *)((int)unaff_A6 + 0x4e) = *(ushort *)((int)unaff_A6 + 0x4e) >> 1;
  *unaff_A6 = 0x1d8e4;
  cVar1 = '\0';
  bVar2 = false;
  cVar3 = '\0';
  candidate_1dd90();
  if (bVar2 || cVar3 != cVar1) {
    *unaff_A6 = candidate_1d908;
  }
  *(char *)(unaff_A6 + 0x23) = *(char *)(unaff_A6 + 0x23) + '\x01';
  uVar4 = 0;
  if ((*(byte *)(unaff_A6 + 0x23) & 7) == 0) {
    candidate_1de0e();
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
