/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ad3e2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ad3e2(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_1ad6e();
  candidate_9ea8();
  candidate_4256();
  candidate_120f8();
  *unaff_A6 = 0xad41a;
  candidate_1abc8();
  iVar2 = unaff_A6[3];
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(iVar2 + 0x1e);
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + 2;
  *(ushort *)(unaff_A6 + 0xf) = (ushort)*(byte *)(iVar2 + 0x82) + *(short *)(unaff_A6 + 0xf);
  uVar3 = *(ushort *)(unaff_A6 + 0x10);
  bVar4 = CARRY2((ushort)*(byte *)(iVar2 + 0x83),uVar3);
  *(ushort *)(unaff_A6 + 0x10) = *(byte *)(iVar2 + 0x83) + uVar3;
  candidate_427a();
  if (bVar4) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
