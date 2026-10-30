/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0aa3de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_aa3de(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  ushort uVar4;
  ushort uVar5;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar6;

  sVar3 = *(short *)(unaff_A6[3] + 0x70);
  *(short *)(unaff_A6 + 0x1c) = sVar3;
  if (*(int *)((short)((sVar3 + -1) * 4) + 0xe6862) != 0) {
    candidate_4256();
  }
  *unaff_A6 = 0xaa412;
  uVar4 = *(ushort *)(unaff_A6[3] + 0x70);
  uVar5 = *(ushort *)(unaff_A6 + 0x1c);
  if (uVar4 != uVar5) {
    *unaff_A6 = candidate_aa3de;
  }
  uVar6 = uVar4 == uVar5 && uVar4 < uVar5;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_44f6();
  candidate_427a();
  candidate_409a();
  if (!(bool)uVar6) {
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
