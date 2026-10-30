/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cbd5c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cbd5c(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  candidate_4256();
  *unaff_A6 = 0xcbd78;
  uVar3 = 0;
  candidate_cb878();
  candidate_427a();
  if ((bool)uVar3) {
    candidate_1da40();
    candidate_4192();
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
