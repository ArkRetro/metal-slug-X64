/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dd802.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dd802(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_9ea8();
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -1;
  candidate_4256();
  *unaff_A6 = 0xdd82a;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  if (!(bool)uVar3) {
    candidate_1945c();
    if (!(bool)uVar3) {
      return;
    }
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
