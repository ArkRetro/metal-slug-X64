/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03cb3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x0003cd88) */
/* WARNING: Removing unreachable block (ram,0x0003cdd2) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_3cb3a(void)

{
  byte bVar1;
  code cVar2;
  byte bVar3;
  char cVar4;
  code cVar5;
  int unaff_A5;

  bVar3 = ~DAT_00380000 & 0xf;
  bVar1 = *(byte *)(unaff_A5 + 0x4366);
  *(byte *)(unaff_A5 + 0x4366) = bVar3;
  *(byte *)(unaff_A5 + 0x4367) = bVar3 & (bVar3 ^ bVar1);
  bVar3 = ~DAT_00300000;
  bVar1 = *(byte *)(unaff_A5 + 0x436a);
  *(byte *)(unaff_A5 + 0x4369) = bVar1;
  *(byte *)(unaff_A5 + 0x436a) = bVar3;
  bVar1 = bVar3 & (bVar3 ^ bVar1);
  *(byte *)(unaff_A5 + 0x436b) = bVar1;
  if (bVar1 == 0) {
    cVar4 = *(char *)(unaff_A5 + 0x436d) + -1;
    *(char *)(unaff_A5 + 0x436d) = cVar4;
    if (cVar4 == '\0') {
      *(byte *)(unaff_A5 + 0x436c) = bVar3;
      *(undefined1 *)(unaff_A5 + 0x436d) = DAT_0010c381;
    }
    else {
      *(undefined1 *)(unaff_A5 + 0x436c) = 0;
    }
  }
  else {
    *(byte *)(unaff_A5 + 0x436c) = bVar1;
    *(undefined1 *)(unaff_A5 + 0x436d) = DAT_0010c380;
  }
  bVar3 = ~DAT_00340000;
  bVar1 = *(byte *)(unaff_A5 + 0x4370);
  *(byte *)(unaff_A5 + 0x436f) = bVar1;
  *(byte *)(unaff_A5 + 0x4370) = bVar3;
  bVar1 = bVar3 & (bVar3 ^ bVar1);
  *(byte *)(unaff_A5 + 0x4371) = bVar1;
  if (bVar1 == 0) {
    cVar4 = *(char *)(unaff_A5 + 0x4373) + -1;
    *(char *)(unaff_A5 + 0x4373) = cVar4;
    if (cVar4 == '\0') {
      *(byte *)(unaff_A5 + 0x4372) = bVar3;
      *(undefined1 *)(unaff_A5 + 0x4373) = DAT_0010c381;
    }
    else {
      *(undefined1 *)(unaff_A5 + 0x4372) = 0;
    }
  }
  else {
    *(byte *)(unaff_A5 + 0x4372) = bVar1;
    *(undefined1 *)(unaff_A5 + 0x4373) = DAT_0010c380;
  }
  if (*(char *)(unaff_A5 + 0x4382) != '\0') {
    if (*(char *)(unaff_A5 + 0x4382) != '\x01') {
      *(code *)(unaff_A5 + 0x435b) = FUN_0000435c[unaff_A5];
      FUN_0000435c[unaff_A5] = (code)0x0;
      *(undefined1 *)(unaff_A5 + 0x435d) = 0;
      cVar4 = *(char *)(unaff_A5 + 0x435f) + -1;
      *(char *)(unaff_A5 + 0x435f) = cVar4;
      if (cVar4 == '\0') {
        *(undefined1 *)(unaff_A5 + 0x435e) = 0;
        *(undefined1 *)(unaff_A5 + 0x435f) = DAT_0010c381;
      }
      else {
        *(undefined1 *)(unaff_A5 + 0x435e) = 0;
      }
      *(undefined1 *)(unaff_A5 + 0x4361) = *(undefined1 *)(unaff_A5 + 0x4362);
      *(undefined1 *)(unaff_A5 + 0x4362) = 0;
      *(undefined1 *)(unaff_A5 + 0x4363) = 0;
      cVar4 = *(char *)(unaff_A5 + 0x4365) + -1;
      *(char *)(unaff_A5 + 0x4365) = cVar4;
      if (cVar4 == '\0') {
        candidate_4364[unaff_A5] = (code)0x0;
        *(undefined1 *)(unaff_A5 + 0x4365) = DAT_0010c381;
      }
      else {
        candidate_4364[unaff_A5] = (code)0x0;
      }
      return;
    }
    if (_DAT_00000180 == 0) {
      candidate_3ce8e();
    }
    else {
      candidate_3ce00();
    }
    cVar2 = *(code *)(unaff_A5 + 0x4374);
    cVar5 = FUN_0000435c[unaff_A5];
    *(code *)(unaff_A5 + 0x435b) = cVar5;
    FUN_0000435c[unaff_A5] = cVar2;
    bVar1 = (byte)cVar2 & ((byte)cVar2 ^ (byte)cVar5);
    *(byte *)(unaff_A5 + 0x435d) = bVar1;
    if (bVar1 == 0) {
      cVar4 = *(char *)(unaff_A5 + 0x435f) + -1;
      *(char *)(unaff_A5 + 0x435f) = cVar4;
      if (cVar4 == '\0') {
        *(code *)(unaff_A5 + 0x435e) = cVar2;
        *(undefined1 *)(unaff_A5 + 0x435f) = DAT_0010c381;
      }
      else {
        *(undefined1 *)(unaff_A5 + 0x435e) = 0;
      }
    }
    else {
      *(byte *)(unaff_A5 + 0x435e) = bVar1;
      *(undefined1 *)(unaff_A5 + 0x435f) = DAT_0010c380;
    }
    cVar2 = *(code *)(unaff_A5 + 0x4375);
    bVar1 = *(byte *)(unaff_A5 + 0x4362);
    *(byte *)(unaff_A5 + 0x4361) = bVar1;
    *(code *)(unaff_A5 + 0x4362) = cVar2;
    cVar5 = (code)((byte)cVar2 & ((byte)cVar2 ^ bVar1));
    *(code *)(unaff_A5 + 0x4363) = cVar5;
    if (cVar5 == (code)0x0) {
      cVar4 = *(char *)(unaff_A5 + 0x4365) + -1;
      *(char *)(unaff_A5 + 0x4365) = cVar4;
      if (cVar4 == '\0') {
        candidate_4364[unaff_A5] = cVar2;
        *(undefined1 *)(unaff_A5 + 0x4365) = DAT_0010c381;
      }
      else {
        candidate_4364[unaff_A5] = (code)0x0;
      }
    }
    else {
      candidate_4364[unaff_A5] = cVar5;
      *(undefined1 *)(unaff_A5 + 0x4365) = DAT_0010c380;
    }
    return;
  }
  *(undefined1 *)(unaff_A5 + 0x435a) = DAT_0010fd94;
  cVar5 = (code)~DAT_00300000;
  cVar2 = FUN_0000435c[unaff_A5];
  *(code *)(unaff_A5 + 0x435b) = cVar2;
  FUN_0000435c[unaff_A5] = cVar5;
  bVar1 = (byte)cVar5 & ((byte)cVar5 ^ (byte)cVar2);
  *(byte *)(unaff_A5 + 0x435d) = bVar1;
  if (bVar1 == 0) {
    cVar4 = *(char *)(unaff_A5 + 0x435f) + -1;
    *(char *)(unaff_A5 + 0x435f) = cVar4;
    if (cVar4 == '\0') {
      *(code *)(unaff_A5 + 0x435e) = cVar5;
      *(undefined1 *)(unaff_A5 + 0x435f) = DAT_0010c381;
    }
    else {
      *(undefined1 *)(unaff_A5 + 0x435e) = 0;
    }
  }
  else {
    *(byte *)(unaff_A5 + 0x435e) = bVar1;
    *(undefined1 *)(unaff_A5 + 0x435f) = DAT_0010c380;
  }
  *(undefined1 *)(unaff_A5 + 0x4360) = DAT_0010fd9a;
  cVar5 = (code)~DAT_00340000;
  bVar1 = *(byte *)(unaff_A5 + 0x4362);
  *(byte *)(unaff_A5 + 0x4361) = bVar1;
  *(code *)(unaff_A5 + 0x4362) = cVar5;
  cVar2 = (code)((byte)cVar5 & ((byte)cVar5 ^ bVar1));
  *(code *)(unaff_A5 + 0x4363) = cVar2;
  if (cVar2 == (code)0x0) {
    cVar4 = *(char *)(unaff_A5 + 0x4365) + -1;
    *(char *)(unaff_A5 + 0x4365) = cVar4;
    if (cVar4 == '\0') {
      candidate_4364[unaff_A5] = cVar5;
      *(undefined1 *)(unaff_A5 + 0x4365) = DAT_0010c381;
    }
    else {
      candidate_4364[unaff_A5] = (code)0x0;
    }
  }
  else {
    candidate_4364[unaff_A5] = cVar2;
    *(undefined1 *)(unaff_A5 + 0x4365) = DAT_0010c380;
  }
  return;
}
