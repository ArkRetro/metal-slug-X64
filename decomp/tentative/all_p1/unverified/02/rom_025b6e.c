/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x025b6e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_25b6e(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_9f88();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1301;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1301;
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  candidate_4256();
  *unaff_A6 = 0x25baa;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar3) {
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
