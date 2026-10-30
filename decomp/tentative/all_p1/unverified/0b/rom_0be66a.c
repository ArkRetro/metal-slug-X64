/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0be66a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_be66a(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_9f88();
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  candidate_29c6();
  candidate_4256();
  *unaff_A6 = 0xbe6a6;
  uVar3 = 0;
  candidate_1abc8();
  candidate_be446();
  if ((bool)uVar3) {
    return;
  }
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = 0xbe684;
    uVar3 = 0;
  }
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = *(undefined1 *)(unaff_A6[3] + 0x5a);
    uVar3 = 0;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xbf;
    candidate_406b6();
  }
  candidate_409a();
  if (!(bool)uVar3) {
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
