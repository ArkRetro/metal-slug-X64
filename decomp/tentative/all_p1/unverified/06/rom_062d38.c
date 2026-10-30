/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x062d38.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_62d38(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  candidate_4256();
  bVar4 = false;
  candidate_40bc();
  if (bVar4) {
    candidate_1b220();
  }
  sVar3 = 4;
  do {
    bVar4 = false;
    candidate_40bc();
    if (bVar4) {
      candidate_1b220();
    }
    sVar3 = sVar3 + -1;
  } while (sVar3 != -1);
  *unaff_A6 = 0x62d9c;
  uVar5 = 0;
  candidate_1abc8();
  candidate_140c0();
  candidate_427a();
  candidate_1945c();
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
