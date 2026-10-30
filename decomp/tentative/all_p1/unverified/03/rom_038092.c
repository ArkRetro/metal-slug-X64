/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x038092.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_38092(void)

{
  code *pcVar1;
  int iVar2;
  undefined4 extraout_D1;
  char cVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  unaff_A6[0xc] = 0xfffd0000;
  candidate_38016();
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
  *unaff_A6 = 0x380ae;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(char *)((int)unaff_A6 + 0x96) = *(char *)((int)unaff_A6 + 0x96) + '\x02';
    candidate_2e7c();
    unaff_A6[0xd] = extraout_D1;
    cVar3 = '\x01';
    if (-1 < *(char *)((int)unaff_A6 + 0x96)) {
      cVar3 = -1;
    }
    *(char *)(unaff_A6 + 0x26) = cVar3 + *(char *)(unaff_A6 + 0x26);
    candidate_377da();
    candidate_37854();
    candidate_1ac38();
    candidate_37512();
    candidate_427a();
    return;
  }
  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x37ab4;
  uVar4 = 0;
  candidate_1abc8();
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
