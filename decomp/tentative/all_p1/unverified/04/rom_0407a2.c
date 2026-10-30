/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0407a2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_407a2(void)

{
  char cVar3;
  uint uVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_4097a();
  cVar3 = candidate_29c6();
  if (cVar3 < '\0') {
    uVar1 = candidate_29c6();
    unaff_A6[0xc] = uVar1 & 0xffff;
    *(uint *)((int)unaff_A6 + 0x76) = uVar1 & 0xffff;
    bVar4 = false;
    candidate_29c6();
    if (bVar4) {
      unaff_A6[0xc] = -unaff_A6[0xc];
      *(int *)((int)unaff_A6 + 0x76) = -*(int *)((int)unaff_A6 + 0x76);
    }
    uVar1 = candidate_29c6();
    unaff_A6[0xd] = (uVar1 & 0xffff) * 4 + 0x30000;
    bVar4 = false;
    uVar2 = candidate_29c6();
    if (bVar4) {
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
    }
    *(ushort *)((int)unaff_A6 + 0x26) = (uVar2 & 0x3f) + 0x80;
    candidate_4256();
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
    *unaff_A6 = candidate_40d06;
    return;
  }
  candidate_4256();
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0x20000;
  *unaff_A6 = 0x40df0;
  return;
}
