/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0be700.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_be700(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  candidate_4256();
  *unaff_A6 = 0xbe736;
  uVar3 = 0;
  candidate_1abc8();
  candidate_be446();
  if ((bool)uVar3) {
    return;
  }
  candidate_427a();
  if (*(char *)(unaff_A6[3] + 0x5a) != '\0') {
    *unaff_A6 = candidate_be770;
  }
  bVar4 = false;
  candidate_409a();
  if (!bVar4) {
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
