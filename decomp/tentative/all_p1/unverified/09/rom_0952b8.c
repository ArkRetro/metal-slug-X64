/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0952b8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_952b8(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  unaff_A6[0xc] =
       unaff_A6[0x1c] + (*(int *)(unaff_A6[1] + 0x3c) - unaff_A6[0xf] >> 7) + unaff_A6[0xc];
  uVar4 = unaff_A6[0x1d] + (*(int *)(unaff_A6[1] + 0x40) - unaff_A6[0x10] >> 7);
  uVar3 = unaff_A6[0xd];
  uVar5 = CARRY4(uVar4,uVar3);
  unaff_A6[0xd] = uVar4 + uVar3;
  candidate_1ac38();
  candidate_427a();
  if ((bool)uVar5) {
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
