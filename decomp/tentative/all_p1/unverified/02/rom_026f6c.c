/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x026f6c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_26f6c(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_4256();
  (**(code **)((short)((*(byte *)((short)(*(byte *)((int)unaff_A6 + 0x87) & 0x1f) + 0x27df4) & 7) <<
                      2) + 0x27e14))();
  if (*(char *)((int)unaff_A6 + 0x87) < '\x02') {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0;
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  }
  candidate_1ad6e();
  *unaff_A6 = 0x26fce;
  if (*(char *)(unaff_A6[3] + 0x88) == *(char *)(unaff_A6 + 0x22)) {
    candidate_27698();
    if (*(char *)((int)unaff_A6 + 0x12) != -1) {
      *(char *)((int)unaff_A6 + 0x12) = *(char *)((int)unaff_A6 + 0x12) + '\b';
    }
    uVar4 = *(char *)((int)unaff_A6 + 0x13) != -1;
    if (*(char *)((int)unaff_A6 + 0x13) != -1) {
      bVar3 = *(byte *)((int)unaff_A6 + 0x13);
      uVar4 = 0xf7 < bVar3;
      *(byte *)((int)unaff_A6 + 0x13) = bVar3 + 8;
    }
    candidate_1abc8();
    candidate_427a();
    if ((bool)uVar4) {
      *unaff_A6 = 0x27016;
    }
    return;
  }
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
