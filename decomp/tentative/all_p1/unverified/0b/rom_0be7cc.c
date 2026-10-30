/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0be7cc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_be7cc(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  candidate_4256();
  *unaff_A6 = 0xbe7fc;
  uVar3 = 0;
  candidate_1abc8();
  candidate_be446();
  if ((bool)uVar3) {
    return;
  }
  candidate_427a();
  bVar4 = false;
  if ((*(short *)(unaff_A6[3] + 0x6e) == 0) && (candidate_409a(), bVar4)) {
    return;
  }
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
