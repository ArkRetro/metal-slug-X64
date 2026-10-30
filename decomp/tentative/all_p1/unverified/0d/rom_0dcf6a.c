/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dcf6a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dcf6a(void)

{
  code *pcVar1;
  byte bVar2;
  undefined4 uVar3;
  short sVar4;
  int iVar5;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;

  candidate_1ac38();
  candidate_427a();
  bVar2 = *(byte *)(unaff_A6 + 0x28);
  bVar6 = bVar2 < 0x60;
  *(byte *)(unaff_A6 + 0x28) = bVar2 + 0xa0;
  if (bVar6) {
    candidate_40bc();
    candidate_1b220();
    iVar5 = unaff_A6[0xd];
    *(int *)(extraout_A0 + 0x30) = (int)unaff_A6[0xc] >> 1;
    *(int *)(extraout_A0 + 0x34) = iVar5 >> 1;
    uVar3 = 0xc0000;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      uVar3 = 0xfff40000;
    }
    *(undefined4 *)(extraout_A0 + 0x82) = uVar3;
    *(undefined4 *)(extraout_A0 + 0x86) = 0xfffee000;
    bVar6 = false;
  }
  candidate_4c24();
  if (bVar6) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    sVar4 = candidate_49e8();
    bVar6 = false;
    if (sVar4 == 0) {
      candidate_1945c();
      if (!bVar6) {
        return;
      }
      goto candidate_4188;
    }
  }
  candidate_120f8();
  bVar6 = false;
  candidate_40bc();
  if (bVar6) {
    candidate_1b220();
  }
candidate_4188:
  candidate_4126();
  iVar5 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar5 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
