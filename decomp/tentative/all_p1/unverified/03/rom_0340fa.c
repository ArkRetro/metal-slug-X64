/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0340fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_340fa(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + 1;
  uVar4 = 0;
  uVar3 = *(ushort *)((int)unaff_A6 + 0x6e) & 7;
  *(ushort *)((int)unaff_A6 + 0x6e) = uVar3;
  if (uVar3 == 0) {
    candidate_34124();
  }
  candidate_1ac38();
  candidate_427a();
  if ((bool)uVar4) {
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
