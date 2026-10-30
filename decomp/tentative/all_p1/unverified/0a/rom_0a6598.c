/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a6598.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_a6598(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined8 uVar4;

  candidate_1ab98();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  candidate_a678a();
  candidate_29c6();
  candidate_29f2();
  uVar4 = candidate_2e7c();
  *(int *)((int)unaff_A6 + 0x7e) = unaff_A6[10] + (int)((ulonglong)uVar4 >> 0x20);
  unaff_A6[0x20] = unaff_A6[0xb] + (int)uVar4;
  *unaff_A6 = 0xa65f6;
  uVar3 = candidate_a6918();
  *(undefined2 *)(unaff_A6 + 0xc) = uVar3;
  uVar3 = candidate_a6918();
  *(undefined2 *)(unaff_A6 + 0xd) = uVar3;
  candidate_1ac38();
  candidate_427a();
  if ((*(ushort *)(unaff_A6 + 0xd) | *(ushort *)(unaff_A6 + 0xc)) == 0) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return *(ushort *)(unaff_A6 + 0xd) | *(ushort *)(unaff_A6 + 0xc);
}
