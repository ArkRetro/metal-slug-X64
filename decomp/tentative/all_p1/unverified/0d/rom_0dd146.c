/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dd146.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dd146(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  byte bVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar5;
  bool bVar6;

  candidate_dd472();
  candidate_1ad6e();
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x9e) = 0xff;
  *unaff_A6 = 0xdd170;
  candidate_dd4b4();
  candidate_dd44c();
  bVar3 = *(byte *)(unaff_A6[3] + 0x61);
  if (bVar3 != 1) {
    bVar4 = *(byte *)((int)unaff_A6 + 0x9e);
    *(byte *)((int)unaff_A6 + 0x9e) = bVar3;
    uVar5 = bVar3 < bVar4;
    if (bVar3 != bVar4) {
      if (bVar3 == 2) {
        candidate_4256();
        bVar6 = false;
        candidate_40bc();
        if (bVar6) {
          candidate_1b220();
        }
        bVar6 = false;
        candidate_40bc();
        if (bVar6) {
          candidate_1b220();
        }
        bVar6 = false;
        candidate_40bc();
        if (bVar6) {
          candidate_1b220();
        }
      }
      uVar5 = bVar3 < 3;
      if (bVar3 == 3) {
        uVar5 = 0;
        candidate_4256();
      }
    }
    candidate_1abc8();
    candidate_44f6();
    candidate_427a();
    if ((bool)uVar5) {
      iVar2 = unaff_A6[3];
      *(undefined1 *)(iVar2 + 0x60) = 1;
      if (*(char *)(iVar2 + 0xa1) == '\0') goto candidate_4188;
      candidate_4256();
    }
    return;
  }
candidate_4188:
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
