/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0981ec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_981ec(void)

{
  ushort uVar1;
  ushort uVar2;
  char cVar3;
  short sVar4;
  undefined4 *unaff_A6;
  bool bVar5;
  bool bVar6;

  candidate_98770();
  candidate_9861e();
  if ((*(short *)(unaff_A6 + 0x21) != 0) &&
     (sVar4 = *(short *)(unaff_A6 + 0x21) + -1, *(short *)(unaff_A6 + 0x21) = sVar4, sVar4 == 0)) {
    *(byte *)(unaff_A6 + 0x23) = *(byte *)(unaff_A6 + 0x23) | 8;
  }
  bVar6 = false;
  if (*(char *)((int)unaff_A6 + 0x91) != '\0') {
    cVar3 = *(char *)((int)unaff_A6 + 0x91);
    bVar6 = cVar3 == '\0';
    *(char *)((int)unaff_A6 + 0x91) = cVar3 + -1;
  }
  candidate_4c24();
  if (bVar6) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(byte *)((int)unaff_A6 + 0x91) = *(byte *)((int)unaff_A6 + 0x91) | 3;
  }
  uVar1 = *(ushort *)((int)unaff_A6 + 0x4e);
  uVar2 = *(ushort *)(unaff_A6 + 0x1e);
  bVar6 = uVar1 >= uVar2;
  if (!bVar6 || uVar1 == uVar2) {
    *(byte *)((int)unaff_A6 + 0x91) = *(byte *)((int)unaff_A6 + 0x91) | 3;
  }
  bVar5 = (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0;
  if (!bVar5) {
    *unaff_A6 = candidate_981f8;
  }
  bVar5 = bVar5 && ((bVar6 && uVar1 != uVar2) && uVar1 < uVar2);
  candidate_1945c();
  if (bVar5) {
    *unaff_A6 = 0x9841e;
  }
  return;
}
